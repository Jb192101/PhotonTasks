/*
Задание 17

Дата выполнения: 11 октября 2024
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <string>
#include <sstream>
#include <cmath>

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

struct Data {
    int k;
    double S;
    double M;
    double O;
    std::vector<std::vector<double>> matrix;
};

int pixelsOffset;
int width;
int height;
int new_width;
int new_height;
int colorsChannels;
int row_size;
int new_row_size;
Data dataR;
int k;

void writeHeader(std::ifstream& input_file, std::ofstream& output_file, BITMAPFILEHEADER& fileHeader, BITMAPINFOHEADER& fileInfoHeader) {
    output_file.write((char*)&fileHeader, sizeof(fileHeader));
    output_file.write((char*)&fileInfoHeader, sizeof(fileInfoHeader));

    char* temp_info = new char[pixelsOffset - 54];
    input_file.seekg(54);
    input_file.read(temp_info, pixelsOffset - 54);
    output_file.write(temp_info, pixelsOffset - 54);
    delete[] temp_info;
}

std::vector<char> createPixelsVector(const std::vector<char>& pixels) {
    std::vector<char> new_pixels(new_row_size * new_height + new_width * colorsChannels);
    for (int i = 0; i < height + dataR.k; i++) {
        for (int j = 0; j < width + dataR.k; j++) {
            int i_2 = i - dataR.k / 2;
            int j_2 = j - dataR.k / 2;

            if (i < dataR.k / 2) {
                i_2 = (dataR.k / 2 - i);
            }

            if (j < dataR.k / 2) {
                j_2 = (dataR.k / 2 - j);
            }

            if (i >= height + dataR.k / 2) {
                i_2 = i - dataR.k - 1;
            }

            if (j >= width + dataR.k / 2) {
                j_2 = j - dataR.k - 1;
            }

            for (int c = 0; c < colorsChannels; c++) {
                new_pixels[i * new_row_size + j * colorsChannels + c] = pixels[i_2 * row_size + j_2 * colorsChannels + c];
            }
        }
    }

    return new_pixels;
}

void medianFilterSTDnth(std::ofstream& output_file_nth_element, std::vector<char>& special_pixels) {
    std::vector<char> n_pixels(row_size * height + width * colorsChannels);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::vector<std::vector<char>> vicinity(colorsChannels);
            for (int a = i - k / 2; a <= i + k / 2; a++) {
                for (int b = j - k / 2; b <= j + k / 2; b++) {
                    for (int c = 0; c < colorsChannels; c++) {
                        vicinity[c].push_back(special_pixels[(a + k / 2) * new_row_size + (b + k / 2) * colorsChannels + c]);
                    }
                }
            }

            for (int iii = 0; iii < colorsChannels; iii++) {
                std::nth_element(vicinity[iii].begin(), vicinity[iii].begin() + vicinity[iii].size() / 2, vicinity[iii].end());
                n_pixels[i * row_size + j * colorsChannels + iii] = vicinity[iii][vicinity[iii].size() / 2];
            }
        }
    }

    output_file_nth_element.write(n_pixels.data(), row_size * height + width * colorsChannels);
    output_file_nth_element.close();
}

void medianFilterSTDsort(std::ofstream& output_file_sort, std::vector<char>& special_pixels) {
    std::vector<char> n_pixels(row_size * height + width * colorsChannels);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::vector<std::vector<char>> vicinity(colorsChannels);
            for (int a = i - k / 2; a <= i + k / 2; a++) {
                for (int b = j - k / 2; b <= j + k / 2; b++) {
                    for (int c = 0; c < colorsChannels; c++) {
                        vicinity[c].push_back(special_pixels[(a + k / 2) * new_row_size + (b + k / 2) * colorsChannels + c]);
                    }
                }
            }

            for (int iii = 0; iii < colorsChannels; iii++) {
                std::sort(vicinity[iii].begin(), vicinity[iii].end());
                n_pixels[i * row_size + j * colorsChannels + iii] = vicinity[iii][vicinity[iii].size() / 2];
            }
        }
    }

    output_file_sort.write(n_pixels.data(), row_size * height + width * colorsChannels);
    output_file_sort.close();
}

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
    width = fileInfoHeader.biWidth;
    height = fileInfoHeader.biHeight;
    new_width = width + dataR.k;
    new_height = height + dataR.k;
    row_size = std::floor((fileInfoHeader.biBitCount * width + 31) / 32) * 4;
    new_row_size = std::floor((fileInfoHeader.biBitCount * new_width + 31) / 32) * 4;

    std::string PATH_1 = "sort_" + OUTPUT_PATH;
    std::string PATH_2 = "nth_" + OUTPUT_PATH;

    std::ofstream output_file_sort(PATH_1, std::ios::binary);
    std::ofstream output_file_nth_element(PATH_2, std::ios::binary);

    if (!(output_file_sort.is_open()) || !(output_file_nth_element.is_open())) {
        std::cout << "Error: can't open one of output files";
        input_file.close();
        output_file_sort.close();
        output_file_nth_element.close();
        exit(1);
    }

    writeHeader(input_file, output_file_sort, fileHeader, fileInfoHeader);
    writeHeader(input_file, output_file_nth_element, fileHeader, fileInfoHeader);

    std::vector<char> pixels(row_size * height + width * colorsChannels);
    input_file.read(pixels.data(), row_size * height + width * colorsChannels);

    input_file.close();

    auto start1 = std::chrono::high_resolution_clock::now();
    std::vector<char> special_pixels = createPixelsVector(pixels);
    medianFilterSTDsort(output_file_sort, special_pixels);
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_sort = end1 - start1;

    std::cout << "Time of median filter (std::sort): " << time_sort.count() << " seconds" << std::endl;

    auto start2 = std::chrono::high_resolution_clock::now();
    medianFilterSTDnth(output_file_nth_element, special_pixels);
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_nth_element = end2 - start2;

    std::cout << "Time of median filter (std::nth_element): " << time_nth_element.count() << " seconds" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cout << "Error: count of input files is not correct";
        return 1;
    }

    std::string INPUT_PATH = argv[1];
    std::string OUTPUT_PATH = argv[2];
    std::string str_k = argv[3];
    k = std::stoi(str_k);

    medianFilter(INPUT_PATH, OUTPUT_PATH);

    return 0;
}

