/*
Задание 16

Дата выполнения: 30 сентября 2024
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <string>

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

void genMatrix(double S, int k) {
    int MSize = k;
    std::vector<std::vector<double>> gaussian_matrix(MSize, std::vector<double>(MSize));

    for (int i = 0; i < MSize; ++i) {
        for (int j = 0; j < MSize; ++j) {
            gaussian_matrix[i][j] = 1.0 / (2 * 3.141592 * S * S) * exp(-((i - (k / 2)) * (i - (k / 2)) + (j - (k / 2)) * (j - (k / 2))) / (2 * S * S));
        }
    }

    dataR.matrix = gaussian_matrix;
}

void MatrixNormalization() {
    double sumUn = 0.0;

    for (int i = 0; i < dataR.k; i++) {
        for (int j = 0; j < dataR.k; j++) {
            sumUn += dataR.matrix[i][j];
        }
    }

    if (dataR.k < 15) {
        std::cout << "The sum of the elements of an unnormalized Gaussian matrix: " << sumUn << std::endl;
        std::cout << std::endl;
    }

    for (int i = 0; i < dataR.k; i++) {
        for (int j = 0; j < dataR.k; j++) {
            dataR.matrix[i][j] /= sumUn;
        }
    }
}

void SaveData() {
    std::ostringstream filename;
    filename << "data_" << dataR.k << "_" << dataR.S << "_" << ".txt";
    std::ofstream file(filename.str());

    file << "k = " << dataR.k << std::endl;
    file << "S = " << dataR.S << std::endl;
    file << "M = " << dataR.M << std::endl;
    file << "O = " << dataR.O << std::endl;
    file << std::endl;

    file << "Коэффициенты матрицы A размерностью " << dataR.k << "x" << dataR.k << ":" << std::endl;
    for (int i = 0; i < dataR.k; i++) {
        for (int j = 0; j < dataR.k; j++) {
            file << dataR.matrix[i][j] << " ";
        }
        file << std::endl;
    }

    file << std::endl;

    std::cout << "Input the data at file succesfully complete" << std::endl;

    file.close();
}

void writeHeader(std::ifstream& input_file, std::ofstream& output_file, BITMAPFILEHEADER& fileHeader, BITMAPINFOHEADER& fileInfoHeader) {
    output_file.write((char*)&fileHeader, sizeof(fileHeader));
    output_file.write((char*)&fileInfoHeader, sizeof(fileInfoHeader));

    char* temp_info = new char[pixelsOffset - 54];
    input_file.seekg(54);
    input_file.read(temp_info, pixelsOffset - 54);
    output_file.write(temp_info, pixelsOffset - 54);
    delete[] temp_info;
}

void filterImage(std::ofstream& output_file, const std::vector<char>& pixels) {
    std::vector<char> new_pixels(row_size * height + width * colorsChannels);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::vector<double> average(colorsChannels);
            for (int a = i - dataR.k / 2; a <= i + dataR.k / 2; a++) {
                for (int b = j - dataR.k / 2; b <= j + dataR.k / 2; b++) {
                    for (int c = 0; c < colorsChannels; c++) {
                        average[c] += dataR.matrix[a - i + dataR.k / 2][b - j + dataR.k / 2] * (int)(unsigned char)pixels[(a + dataR.k / 2) * new_row_size + (b + dataR.k / 2) * colorsChannels + c];
                    }
                }
            }

            for (int c = 0; c < colorsChannels; c++) {
                average[c] = dataR.M * average[c] + dataR.O;
                average[c] = std::min(std::max(average[c], 0.0), 255.0);
                new_pixels[i * row_size + j * colorsChannels + c] = static_cast<char>(round(average[c]));
            }

        }
    }

    output_file.write(new_pixels.data(), row_size * height + width * colorsChannels);
    output_file.close();
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

    std::ofstream output_file(OUTPUT_PATH, std::ios::binary);

    if (!(output_file.is_open())) {
        std::cout << "Error: can't open one of output_files";
        input_file.close();
        output_file.close();
        exit(1);
    }

    writeHeader(input_file, output_file, fileHeader, fileInfoHeader);

    std::vector<char> pixels(row_size * height + width * colorsChannels);
    input_file.read(pixels.data(), row_size * height + width * colorsChannels);

    input_file.close();

    auto start_time = std::chrono::high_resolution_clock::now();
    std::vector<char> special_pixels = createPixelsVector(pixels);
    filterImage(output_file, special_pixels);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;
    std::cout << "Time of mirror reflection: " << duration.count() << " seconds" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc < 5) {
        std::cout << "Error: count of input files is not correct";
        return 1;
    }

    std::string INPUT_PATH = argv[1];
    std::string OUTPUT_PATH = argv[2];
    std::string FILE_PATH = argv[3];
    std::string k1 = argv[3];
    std::string S1 = argv[4];

    dataR.k = std::stoi(k1);
    dataR.S = std::stoi(S1);
    dataR.M = 1;
    dataR.O = 0;

    genMatrix(dataR.S, dataR.k);

    if (dataR.k < 15) {
        std::cout << "Received Gaussian matrix:" << std::endl;

        for (int i = 0; i < dataR.k; i++) {
            for (int j = 0; j < dataR.k; j++) {
                std::cout << dataR.matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }

    MatrixNormalization();

    if (dataR.k < 15) {
        std::cout << "Received normalize Gaussian matrix:" << std::endl;

        for (int i = 0; i < dataR.k; i++) {
            for (int j = 0; j < dataR.k; j++) {
                std::cout << dataR.matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    medianFilter(INPUT_PATH, OUTPUT_PATH);
    SaveData();

    return 0;
}

