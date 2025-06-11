/*
Задание 14

Дата выполнения: 2 сентября 2024
*/

#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>
#include <vector>
#include <fstream>

#pragma pack(2)

struct BITMAPFILEHEADER {
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
};

struct BITMAPINFOHEADER {
    unsigned int biSize;
    int biWidth;
    int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
};

int pixelsOffset;
int w;
int h;
int new_w;
int new_h;
int row_size;
int new_row_size;
int k;
int colorsChannels;

void write_header(std::ifstream& input_file, std::ofstream& output_file, BITMAPFILEHEADER& fileHeader, BITMAPINFOHEADER& fileInfoHeader) {
    output_file.write((char*)&fileHeader, sizeof(fileHeader));
    output_file.write((char*)&fileInfoHeader, sizeof(fileInfoHeader));

    char* temp_info = new char[pixelsOffset - 54];
    input_file.seekg(54);
    input_file.read(temp_info, pixelsOffset - 54);
    output_file.write(temp_info, pixelsOffset - 54);
    delete[] temp_info;
}

void filter_image(std::ofstream& output_file, const std::vector<char>& pixels) {
    std::vector<char> new_pixels(row_size * h + w * colorsChannels);
    std::vector<int> ps(new_row_size * new_h + new_w * colorsChannels);
    for (int i = 0; i < new_h; i++) {
        for (int c = 0; c < colorsChannels; c++) {
            ps[i * new_row_size + c] = (int)(unsigned char)pixels[i * new_row_size + c];
        }
        for (int j = 1; j < new_w; j++) {
            for (int c = 0; c < colorsChannels; c++) {
                ps[i * new_row_size + j * colorsChannels + c] = ps[i * new_row_size + (j - 1) * colorsChannels + c] + \
                    (int)(unsigned char)pixels[i * new_row_size + j * colorsChannels + c];
            }
        }
    }

    std::vector<std::vector<int>> sums_pixels(w);
    std::vector<double> average(colorsChannels);
	
    for (int i = 0; i < k; i++) {
        for (int c = 0; c < colorsChannels; c++) {
            average[c] += ps[i * new_row_size + (k - 1) * colorsChannels];
        }
    }
	
    for (int c = 0; c < colorsChannels; c++) {
        sums_pixels[0].push_back(average[c]);
        average[c] /= (double)(k * k);
        new_pixels[c] = average[c];
    }

    for (int j = 1; j < w; j++) {
        std::vector<double> average(colorsChannels);
		
        for (int i = 0; i < k; i++) {
            for (int c = 0; c < colorsChannels; c++) {
                average[c] += (ps[i * new_row_size + (j + k - 1) * colorsChannels + c] -
                    ps[i * new_row_size + (j - 1) * colorsChannels + c]);
            }
        }
		
        for (int c = 0; c < colorsChannels; c++) {
            sums_pixels[j].push_back(average[c]);
            average[c] /= (double)(k * k);
            new_pixels[j * colorsChannels + c] = average[c];
        }
    }

    for (int i = 1; i < h; i++) {
        for (int j = 0; j < w; j++) {
            std::vector<double> average(colorsChannels);
			
            for (int c = 0; c < colorsChannels; c++) {
                if (j == 0) {
                    average[c] = sums_pixels[j][c] - ps[(i - 1) * new_row_size + (j + k - 1) * colorsChannels + c] + \
                        ps[(i + k - 1) * new_row_size + (j + k - 1) * colorsChannels + c];
                }
                else {
                    average[c] = sums_pixels[j][c] - \
                        (ps[(i - 1) * new_row_size + (j + k - 1) * colorsChannels + c] - ps[(i - 1) * new_row_size + (j - 1) * colorsChannels + c]) + \
                        (ps[(i + k - 1) * new_row_size + (j + k - 1) * colorsChannels + c] - ps[(i + k - 1) * new_row_size + (j - 1) * colorsChannels + c]);
                }

                sums_pixels[j][c] = average[c];
                average[c] /= (double)(k * k);
                new_pixels[i * row_size + j * colorsChannels + c] = average[c];
            }
        }
    }

    output_file.write(new_pixels.data(), row_size * h + w * colorsChannels);
    output_file.close();
}

std::vector<char> pixels_vector_creating(const std::vector<char>& pixels) {
    std::vector<char> new_pixels(new_row_size * new_h + new_w * colorsChannels);
    for (int i = 0; i < h + k; i++) {
        for (int j = 0; j < w + k; j++) {
            int i_2 = i - k / 2;
            int j_2 = j - k / 2;

            if (i < k / 2) {
                i_2 = (k / 2 - i);
            }
            if (j < k / 2) {
                j_2 = (k / 2 - j);
            }
            if (i >= h + k / 2) {
                i_2 = i - k - 1;
            }
            if (j >= w + k / 2) {
                j_2 = j - k - 1;
            }

            for (int c = 0; c < colorsChannels; c++) {
                new_pixels[i * new_row_size + j * colorsChannels + c] = pixels[i_2 * row_size + j_2 * colorsChannels + c];
            }
        }
    }

    return new_pixels;
}

// срединный фильтр
void medianFilter(const std::string INPUT_PATH, const std::string OUTPUT_PATH) {
    std::ifstream input_file(INPUT_PATH, std::ios::binary);

    if (!(input_file.is_open())) {
        std::cout << "Error: can't open input_file";
        input_file.close();
        exit(1);
    }

    BITMAPFILEHEADER fileHeader;
    input_file.read((char*)&fileHeader, sizeof(fileHeader));

    BITMAPINFOHEADER fileInfoHeader;
    input_file.read((char*)&fileInfoHeader, sizeof(fileInfoHeader));

    pixelsOffset = fileHeader.bfOffBits;
    colorsChannels = fileInfoHeader.biBitCount / 8;
	
    w = fileInfoHeader.biWidth;
    h = fileInfoHeader.biHeight;
	
    new_w = w + k;
    new_h = h + k;
	
    row_size = std::floor((fileInfoHeader.biBitCount * w + 31) / 32) * 4;
    new_row_size = std::floor((fileInfoHeader.biBitCount * new_w + 31) / 32) * 4;

    std::string OUTPUTPATH_FOR_FILTER_WITH_ADD_MIRROR_REFLECTION = "__add_mirror_reflection__" + OUTPUT_PATH;

    std::ofstream output_file(OUTPUTPATH_FOR_FILTER_WITH_ADD_MIRROR_REFLECTION, std::ios::binary);

    if (!(output_file.is_open())) {
        std::cout << "Error: can't open one of output_files";
        input_file.close();
        output_file.close();
        exit(1);
    }

    write_header(input_file, output_file, fileHeader, fileInfoHeader);

    std::vector<char> pixels(row_size * h + w * colorsChannels);
    input_file.read(pixels.data(), row_size * h + w * colorsChannels);

    input_file.close();

    auto time1 = std::chrono::high_resolution_clock::now();
	
    std::vector<char> special_pixels = pixels_vector_creating(pixels);
    filter_image(output_file, special_pixels);
	
    auto time2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = time2 - time1;
    std::cout << "Time of process: " << duration.count() << " seconds" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cout << "Error: insufficient amount of input data";
        return 1;
    }

    std::string INPUT = argv[1];
    std::string OUTPUT = argv[2];
    std::string str_k = argv[3];
    k = std::stoi(str_k);

    medianFilter(INPUT, OUTPUT);

    return 0;
}