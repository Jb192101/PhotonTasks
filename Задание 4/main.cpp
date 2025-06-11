/*
Задание 4

Дата выполнения: 21 апреля 2024
*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <string>
#include <math.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "en_US.UTF8");

    // Проверка на открытие файла
    if (argc < 3)
    {
        cout << "Error: count of input files dont equal 3" << endl;
        return 1;
    }

    FILE* streamIn;
    FILE* streamOut;
    streamIn = fopen(argv[1], "rb");
    streamOut = fopen(argv[2], "wb");

    // Проверка на поиск и на формат
    if (streamIn == NULL)
    {
        cout << "Error: File dont finded" << endl;
        return 1;
    }

    if (fgetc(streamIn) != 'B' || fgetc(streamIn) != 'M')
    {
        cout << "Error: file not BMP" << endl;
        return 1;
    }

    unsigned char bmpHeader[52];

    fread(bmpHeader, sizeof(unsigned char), 52, streamIn);

    // Считывание данных
    int width = *(int*)&bmpHeader[16];
    int height = *(int*)&bmpHeader[20];

    unsigned int pixels_offset = *(unsigned int*)&bmpHeader[8];

    int rowsize = ceil(width * 0.75) * 4;

    unsigned char* additional_data = new unsigned char[pixels_offset - 54];

    fread(additional_data, sizeof(unsigned char), pixels_offset - 54, streamIn);

    fputc('B', streamOut);
    fputc('M', streamOut);

    fwrite(bmpHeader, sizeof(unsigned char), 52, streamOut);
    fwrite(additional_data, sizeof(unsigned char), pixels_offset - 54, streamOut);
    delete[] additional_data;

    unsigned char* line = new unsigned char[rowsize];

    // негатив по формуле 
    for (int y = 0; y < height; y++)
    {
        fread(line, sizeof(unsigned char), rowsize, streamIn);

        for (int x = 0; x < width; x++)
        {
            unsigned char pixel[3];
            memcpy(pixel, line + x * 3, 3 * sizeof(unsigned char));

            for (int i = 0; i < 3; ++i)
                pixel[i] = 255 - pixel[i];
            fwrite(pixel, sizeof(unsigned char), 3, streamOut);
        }
        for (int i = 0; i < rowsize - width * 3; i++)
            fputc(0, streamOut);
    }

    delete[] line;

    fclose(streamIn);
    fclose(streamOut);

    return 0;
}
