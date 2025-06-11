/*
Задание 12

Дата выполнения: 19 августа 2024
*/

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <cmath>
#include <vector>
#include <thread>
#include <mutex>
#include <cstdint>

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

BITMAPFILEHEADER fileHeader;
BITMAPINFOHEADER fileInfoHeader;
std::vector<char> pixels;
std::vector<char> new_pixels;
int row_size;
int colorsChannels;
int width;
int height;
int outputWidth;
int outputHeight;
int new_row_size;
double radian;
double cx;
double cy;

std::mutex progress_mutex;
int progress = 0;
int total_tiles = 0;

void rotate_thread(int thread_id, int num_threads, int tile_size) {
    int tile_x = thread_id % (outputWidth / tile_size);
    int tile_y = thread_id / (outputWidth / tile_size);
    int start_tile = thread_id * (outputHeight / tile_size / num_threads);
    int end_tile = (thread_id + 1) * (outputHeight / tile_size / num_threads);
    if (thread_id == num_threads - 1) {
        end_tile = outputHeight / tile_size;
    }

    for (int tile_y = start_tile; tile_y < end_tile; ++tile_y) {
        for (int tile_x = 0; tile_x < outputWidth / tile_size; ++tile_x) {
            for (int y = tile_y * tile_size; y < (tile_y + 1) * tile_size; ++y) {
                for (int x = tile_x * tile_size; x < (tile_x + 1) * tile_size; ++x) {
                    int src_x = (int)((x - outputWidth / 2.0) * cos(radian) - (y - outputHeight / 2.0) * sin(radian) + width / 2.0);
                    int src_y = (int)((x - outputWidth / 2.0) * sin(radian) + (y - outputHeight / 2.0) * cos(radian) + height / 2.0);

                    if (src_x >= 0 && src_x < width && src_y >= 0 && src_y < height) {
                        double src_pos = (double)src_y * row_size + (double)src_x * colorsChannels;
                        double dst_pos = y * new_row_size + x * colorsChannels;

                        for (int c = 0; c < colorsChannels; ++c) {
                            new_pixels[dst_pos + c] = pixels[src_pos + c];
                        }
                    }
                }
            }

        }
    }
    {
        std::lock_guard<std::mutex> lock(progress_mutex);
        while (progress < tile_size) {
            progress++;
            std::cout << "Tile " << progress << " of " << tile_size << " processed" << std::endl;
        }

    }
}

void rotate_bln(const std::string INPUT_PATH, const std::string OUTPUT_PATH, int angle, int num_threads, int tile_size) {
    std::ifstream input_file(INPUT_PATH, std::ios::binary);
    std::ofstream output_file(OUTPUT_PATH, std::ios::binary);

    if (!(input_file.is_open() && output_file.is_open())) {
        std::cout << "Error: can't open one of the files";
        input_file.close();
        output_file.close();
        exit(1);
    }

    input_file.read((char*)&fileHeader, sizeof(fileHeader));
    input_file.read((char*)&fileInfoHeader, sizeof(fileInfoHeader));

    int pixelsOffset = fileHeader.bfOffBits;
    colorsChannels = fileInfoHeader.biBitCount / 8;
    width = fileInfoHeader.biWidth;
    height = fileInfoHeader.biHeight;
    row_size = std::floor((fileInfoHeader.biBitCount * width + 31) / 32) * 4;

    radian = angle * 3.14159265 / 180.0;
    outputWidth = abs(fileInfoHeader.biWidth * fabs(cos(radian)) + fileInfoHeader.biHeight * fabs(sin(radian))) + 12;
    outputHeight = abs(fileInfoHeader.biWidth * fabs(sin(radian)) + fileInfoHeader.biHeight * fabs(cos(radian))) + 12;

    fileInfoHeader.biWidth = outputWidth;
    fileInfoHeader.biHeight = outputHeight;
    new_row_size = std::floor((fileInfoHeader.biBitCount * outputWidth + 31) / 32) * 4;

    output_file.write((char*)&fileHeader, sizeof(fileHeader));
    output_file.write((char*)&fileInfoHeader, sizeof(fileInfoHeader));

    char* temp_info = new char[pixelsOffset - 54];
    input_file.read(temp_info, pixelsOffset - 54);
    output_file.write(temp_info, pixelsOffset - 54);
    delete[] temp_info;
    pixels.resize(height * row_size);
    input_file.read(pixels.data(), pixels.size());

    new_pixels.resize(outputHeight * new_row_size);

    cx = outputWidth / 2;
    cy = outputHeight / 2;


    auto startTime = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(rotate_thread, i, num_threads, tile_size);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = endTime - startTime;

    output_file.write(new_pixels.data(), new_pixels.size());

    std::cout << "Time of successful creating of image: " << duration.count() << " seconds" << std::endl;
    input_file.close();
    output_file.close();
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cout << "Please, enter the PATH to the input file, to the output file, the rotate angle and tile_size";
        return 1;
    }

    std::string input_path = argv[1];
    std::string output_path = argv[2];
    std::string str_angle = argv[3];
    std::string str_tilesize = argv[4];
    int angle = std::stoi(str_angle);
    int tile_size = std::stoi(str_tilesize);

    int num_threads = std::thread::hardware_concurrency();
    rotate_bln(input_path, output_path, angle, num_threads, tile_size);

    return 0;
}
