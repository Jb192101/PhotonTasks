﻿/*
Задание 7

Дата выполнения: 30 апреля 2024
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <string>
#include <chrono>

#pragma pack(2)

// Структуры BMP-файла
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
//

// Масштабирование через прореживание
void thinning_image(const std::string INPUT_PATH, const std::string OUTPUT_PATH, int n) {
    std::ifstream input_file(INPUT_PATH, std::ios::binary);
    std::ofstream output_file(OUTPUT_PATH, std::ios::binary);

    if (!(input_file.is_open() && output_file.is_open())) {
        std::cout << "Error: can't open one of the files";
        input_file.close();
        output_file.close();
        exit(1);
    }

    BITMAPFILEHEADER fileHeader;
    input_file.read((char*)&fileHeader, sizeof(fileHeader));

    BITMAPINFOHEADER fileInfoHeader;
    input_file.read((char*)&fileInfoHeader, sizeof(fileInfoHeader));

    int pixelsOffset = fileHeader.bfOffBits;
    int colorsChannels = fileInfoHeader.biBitCount / 8;
    int width = fileInfoHeader.biWidth;
    int height = fileInfoHeader.biHeight;
    int row_size = std::floor((fileInfoHeader.biBitCount * width + 31) / 32) * 4;

    int new_width = width / n;
    int new_height = height / n;
    int new_row_size = std::floor((fileInfoHeader.biBitCount * new_width + 31) / 32) * 4;

    fileInfoHeader.biWidth = new_width;
    fileInfoHeader.biHeight = new_height;

    output_file.write((char*)&fileHeader, sizeof(fileHeader));
    output_file.write((char*)&fileInfoHeader, sizeof(fileInfoHeader));

    char* temp_info = new char[pixelsOffset - 54];
    input_file.read(temp_info, pixelsOffset - 54);
    output_file.write(temp_info, pixelsOffset - 54);
    delete[] temp_info;

    // Начало отсчёта времени
    auto startTime = std::chrono::high_resolution_clock::now();

    std::vector<char> row(row_size);
    int x = 0;
    std::vector<char> new_pixels(new_row_size * new_height + (new_width + 1) * colorsChannels);
    int count = 0;
    for (int i = n / 2; i < height; i += n) {
        int y = 0;
        input_file.seekg(pixelsOffset + (height <= 0 ? (height - i - 1) : i) * row_size);
        input_file.read(row.data(), row_size);

        for (int j = n / 2; j < width; j += n) {
            for (int k = 0; k < colorsChannels; k++) {
                new_pixels[x * new_row_size + y * colorsChannels + k] = row[j * colorsChannels + k];
            }
            new_pixels[x * new_row_size + y * colorsChannels] = row[j * colorsChannels];
            y++;
        }
        x++;
    }

    output_file.write(new_pixels.data(), new_row_size * new_height + new_width * colorsChannels);

    // Конец отсчёта времени
    auto endTime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = endTime - startTime;
    std::cout << "Time of thinning image:  " << duration.count() << " seconds" << std::endl;

    input_file.close();
    output_file.close();
}

// Масштабирование через усреднение
void averaging_image(const std::string INPUT_PATH, const std::string OUTPUT_PATH, int n) {
    std::ifstream input_file(INPUT_PATH, std::ios::binary);
    std::ofstream output_file(OUTPUT_PATH, std::ios::binary);

    if (!(input_file.is_open() && output_file.is_open())) {
        std::cout << "Error: can't open one of the files";
        input_file.close();
        output_file.close();
        exit(1);
    }

    BITMAPFILEHEADER fileHeader;
    input_file.read((char*)&fileHeader, sizeof(fileHeader));

    BITMAPINFOHEADER fileInfoHeader;
    input_file.read((char*)&fileInfoHeader, sizeof(fileInfoHeader));

    int pixelsOffset = fileHeader.bfOffBits;
    int colorsChannels = fileInfoHeader.biBitCount / 8;
    int width = fileInfoHeader.biWidth;
    int height = fileInfoHeader.biHeight;
    int row_size = std::floor((fileInfoHeader.biBitCount * width + 31) / 32) * 4;

    int new_width = width / n;
    int new_height = height / n;
    int new_row_size = std::floor((fileInfoHeader.biBitCount * new_width + 31) / 32) * 4;

    fileInfoHeader.biWidth = new_width;
    fileInfoHeader.biHeight = new_height;

    output_file.write((char*)&fileHeader, sizeof(fileHeader));
    output_file.write((char*)&fileInfoHeader, sizeof(fileInfoHeader));

    char* temp_info = new char[pixelsOffset - 54];
    input_file.read(temp_info, pixelsOffset - 54);
    output_file.write(temp_info, pixelsOffset - 54);
    delete[] temp_info;

    auto startTime = std::chrono::high_resolution_clock::now();

    std::vector<char> row(row_size);
    std::vector<char> new_pixels(new_row_size * new_height + new_width * colorsChannels);
    std::vector<char> pixels(row_size * height + width * colorsChannels);
    input_file.read(pixels.data(), row_size * height + width * colorsChannels);
    int x_1 = 0;
    std::vector<int> s(colorsChannels);

    for (int x = 0; x <= height - n; x += n) {
        int y_1 = 0;
        for (int y = 0; y <= width - n; y += n) {
            for (int k = 0; k < colorsChannels; k++) {
                s[k] = pixels[0] - pixels[0];
            }
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    for (int k = 0; k < colorsChannels; k++) {
                        if ((int)(pixels[(x + i) * row_size + (y + j) * colorsChannels + k]) >= 0) {
                            s[k] += pixels[(x + i) * row_size + (y + j) * colorsChannels + k];
                        }
                        else {
                            s[k] += 256 + pixels[(x + i) * row_size + (y + j) * colorsChannels + k];
                        }
                    }
                }
            }
            for (int k = 0; k < colorsChannels; k++) {
                new_pixels[x_1 * new_row_size + y_1 * colorsChannels + k] = s[k] / (n * n);
            }
            y_1++;
        }
        x_1++;
    }

    output_file.write(new_pixels.data(), new_row_size * new_height + new_width * colorsChannels);

    auto endTime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = endTime - startTime;
    std::cout << "Time of averaging image:  " << duration.count() << " seconds" << std::endl;

    input_file.close();
    output_file.close();
}


// Основная программа
int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << "Error: input of file path incorrect";
        exit(2);
    }

    std::string INPUT_PATH = argv[1];
    std::string OUTPUT_PATH = argv[2];
    std::string OUTPUTPATH_FOR_THINNING = "thinning_" + OUTPUT_PATH;
    std::string OUTPUTPATH_FOR_AVERAGING = "averaging_" + OUTPUT_PATH;
    std::string str_num = argv[3];
    int num = std::stoi(str_num);

    thinning_image(INPUT_PATH, OUTPUTPATH_FOR_THINNING, num);
    averaging_image(INPUT_PATH, OUTPUTPATH_FOR_AVERAGING, num);


    return 0;
}
