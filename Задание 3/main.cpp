/*
Задание 3

Дата выполнения: 24 марта 2024
*/

#include <iostream>
#include <fstream>
#include <cmath>

// Выравнивание структур
#pragma pack(2)

struct BITMAPFILEHEADER {
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
};

struct BITMAPINFOHEADER {
    unsigned int bSize;
    int bWidth;
    int bHeight;
    unsigned short bPlanes;
    unsigned short bBitCount;
    unsigned int bCompression;
    unsigned int bSizeImage;
    int bXPelsPerMeter;
    int bYPelsPerMeter;
    unsigned int bClrUsed;
    unsigned int bClrImportant;
};

int main() {
	// Ввод файла
    std::string nameFile;
    std::cout << "Enter the name of the BMP file: ";
    std::cin >> nameFile;
    std::ifstream file(nameFile, std::ios::in | std::ios::binary);
	
    if (!file) {
        std::cerr << "Error: file hasn't opening" << std::endl;
        return 1;
    }

    BITMAPFILEHEADER fHeader;
    file.read((char*)&fHeader, sizeof(fileHeader));

    BITMAPINFOHEADER fInfoHeader;
    file.read((char*)&fInfoHeader, sizeof(fInfoHeader));

    int colors_channels = fInfoHeader.biBitCount / 8;
	std::cout << "Dates of Input BMP-file:" << std::endl;
    std::cout << "Image width: " << fInfoHeader.bWidth << " pixels" << std::endl;
    std::cout << "Image height: " << std::abs(fInfoHeader.bHeight) << " pixels" << std::endl;
    std::cout << "Bits per channel: " << fInfoHeader.bBitCount << " bits" << std::endl;
    std::cout << "Number of color channels: " << colors_channels << std::endl;


    int w = fInfoHeader.bWidth;
    int h = fInfoHeader.bHeight;

    for (int i = 0; i < 5; i++) {
        int x, y;
        switch (i) {
        case 0: // Верхний левый
            x = 0; y = 0;
            break;
        case 1: // Верхний правый
            x = w - 1; y = 0;
            break;
        case 2: // Центр
            x = w / 2; y = h / 2;
            break;
        case 3: // Нижний левый
            x = 0; y = h - 1;
            break;
        case 4: // Нижний правый
            x = w - 1; y = h - 1;
            break;
        }

        int row_size = std::floor((fInfoHeader.bBitCount * w + 31) / 32) * 4;
        file.seekg(fHeader.bfOffBits + (h > 0 ? (h - y - 1) : y) * row_size + x * colors_channels);
        char* rgb = new char[colors_channels];
        file.read(rgb, colors_channels);

        std::cout << "Pixel (" << x << "," << y << "): ";

        if (fInfoHeader.bBitCount == 8) {
            for (int j = 0; j < 3; j++) {
                std::cout << (unsigned int)(uint8_t)rgb[0] << " ";
            }
        }
        else {
            for (int j = colors_channels - 1; j >= 0; j--) {
                std::cout << (unsigned int)(uint8_t)rgb[j] << " ";
            }
        }
        std::cout << std::endl;
    }

    file.close();

    return 0;
}