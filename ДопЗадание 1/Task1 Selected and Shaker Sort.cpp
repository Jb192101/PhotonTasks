// Task1 Quick and Shaker Sort.cpp
// Сравнение времени работы сортировки выбором и шейкер сортировки
// Дата выполнения: 3 декабря 2024

// Библиотеки
#include <iostream>
#include <vector>
#include <chrono>

// Генерация рандомных значений
std::vector<__int64> rand_generation(std::vector<__int64> massive, int N)
{
    srand(time(NULL));
    for (int i = 0; i < N; i++)
    {
        massive[i] = rand() % _I64_MAX;
    }

    return massive;
}

// Сортировка выбором O(N^2)
std::chrono::duration<double> selected_sort(std::vector<__int64> massive, int N)
{
    int min; // для индекса

    int l = 0;
    int r = N - 1;

    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = l; i < r; i++)
    {
        int min = i;
        for (int j = i + 1; j <= r; j++)
            if (massive[j] < massive[min])
                min = j;
        std::swap(massive[i], massive[min]);
    }
    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << "Быстрая сортировка : " << std::endl;
    for (int i = 0; i < N; i++)
    {
        std::cout << massive[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    std::chrono::duration<double> timeExecution = t2 - t1;

    return timeExecution;
}

// Шейкер сортировка O(N*log(N))
std::chrono::duration<double> shaker_sort(std::vector<__int64> massive, int N)
{
    int left = 0;
    int right = N - 1;

    auto t1 = std::chrono::high_resolution_clock::now();
    do
    {
        for (int i = right; i > left; i--)
        {
            if (massive[i] < massive[i - 1])
            {
                std::swap(massive[i], massive[i - 1]);
            }
        }

        left++;

        for (int i = left; i < right; i++)
        {
            if (massive[i] > massive[i + 1])
            {
                std::swap(massive[i], massive[i + 1]);
            }
        }

        right--;
    } while (left <= right);

    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << "Шейкер сортировка : " << std::endl;
    for (int i = 0; i < N; i++)
    {
        std::cout << massive[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    std::chrono::duration<double> timeExecution = t2 - t1;

    return timeExecution;
}

int main(int argc, char** argv)
{
    // Предполагается, что данные будут получены через параметры запуска
    // argv[0] - путь
    // argv[1] - значение N
    setlocale(LC_ALL, "RU");

    int N;
    std::chrono::duration<double> time_select;
    std::chrono::duration<double> time_shaker;
    
    //N = (int) argv[1];
    std::cin >> N;

    if (N <= 0)
    {
        std::cout << "Ошибка : некорректный размер массива!" << std::endl;
        return 1;
    }

    std::vector<__int64> massive(N);

    massive = rand_generation(massive, N);

    std::cout << "Исходный массив : " << std::endl;
    for (int i = 0; i < N; i++)
    {
        std::cout << massive[i] << " ";
    }
    std::cout << std::endl;

    time_select = selected_sort(massive, N);
    time_shaker = shaker_sort(massive, N);

    std::cout << "Время работы сортировок : " << std::endl;
    std::cout << "Сортировка выбором (Selected Sort) : " << time_select.count() * 100000 << " миллисекунд(ы)" << std::endl;
    std::cout << "Шейкер сортировка (Shaker Sort) : " << time_shaker.count() * 100000 << " миллисекунд(ы)" << std::endl;

    if (time_select < time_shaker)
    {
        std::cout << "Сортировка выбором работает за меньшее время" << std::endl;
    }
    else if (time_select > time_shaker)
    {
        std::cout << "Шейкер сортировка работает за меньшее время" << std::endl;
    }
    else
    {
        std::cout << "Обе сортировки работают за одинаковое время" << std::endl;
    }

    return 0;
}