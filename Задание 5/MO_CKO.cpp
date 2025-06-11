/*
Задание 5

Дата выполнения: 27 апреля 2024
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>


// Структура ответа
struct Answer {
    std::vector<double> mean;
    std::vector<double> cko;
};

// Функция вычисления СКО и МО
Answer calculate_MO_CKO(const std::string PATH) {
    // Проверка пути файла
    std::ifstream file(PATH, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Error: file cant be identified by path";
        file.close();
        exit(1);
    }
    //

    // Проверка на формат вводимого файла
    char header[54];
    file.read(header, 54);
    if (header[0] != 'B' || header[1] != 'M') {
        std::cout << "Error: file not BMP formate" << std::endl;
        file.close();
        exit(2);
    }
    //

    int pixelsOffset = *(int*)&header[10];
    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    int bitsPerPixel = *(int*)&header[28];
    int colorsChannels = bitsPerPixel / 8;
    int imageSize = width * height * colorsChannels;
    int row_size = std::floor((bitsPerPixel * width + 31) / 32) * 4;

    char* pixels = new char[imageSize];
    file.seekg(pixelsOffset);
    file.read(pixels, imageSize);
    file.close();

    std::vector<double> mean(colorsChannels);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int offset = (x * colorsChannels) + (y * row_size);
            for (int k = 0; k < colorsChannels; k++) {
                mean[k] += (int)(unsigned char)pixels[offset + k];
            }
        }
    }
    // Вычисление МО
    for (int k = colorsChannels - 1; k >= 0; k--) {
        mean[k] /= (double)(width * height);
    }
    //

    // Вычисление СКО
    std::vector<double> standart_devation(colorsChannels);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int offset = (x * colorsChannels) + (y * row_size);
            for (int k = 0; k < colorsChannels; k++) {
                standart_devation[k] += ((int)(unsigned char)pixels[offset + k] - mean[k]) *
                    ((int)(unsigned char)pixels[offset + k] - mean[k]);
            }
        }
    }
    for (int k = 0; k < colorsChannels; k++) {
        standart_devation[k] = std::sqrt(standart_devation[k] / (double)(width * height));
    }
    //

    delete[] pixels;

    Answer result;
    result.mean = mean;
    result.cko = standart_devation;


    return result;
}

// Функция вывода результатов
void output_result(const Answer& answer) {
    std::vector<double> mean = answer.mean;
    std::vector<double> cko = answer.cko;
    std::reverse(mean.begin(), mean.end());
    std::reverse(cko.begin(), cko.end());

    std::cout << "Math. expectation: ";
    for (auto num : mean) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    std::cout << "Mean square deviation: ";
    for (auto num : cko) {
        std::cout << num << " ";
    }
}

// Основная программа
int main(int argc, char* argv[]) {
    std::cout << std::fixed << std::setprecision(10);
    // Проверка на корректный ввод данных
    if (argc != 2) {
        std::cout << "Error: you need to enter the path to the file";
        return 1;
    }

    std::string PathF = argv[1];
    Answer answer = calculate_MO_CKO(PathF);
    output_result(answer);

    return 0;
}