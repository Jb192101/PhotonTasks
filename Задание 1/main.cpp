/*
Задание 1

Дата выполнения: 26 февраля 2024
*/

#include <iostream>
#include <vector>
#include <iomanip>
#include <random>
#include <chrono>
#include <string>

// Набор структур и функций
// Структура 
struct Array_of_str {
  std::string name;
  double percentage;
};

// Функция генерации имени
std::string Name_Gen() {
  std::string name;
  int size = rand() % 10 + 1;
  for (int i = 0; i < size; i++) {
    char c = 'a' + rand() % 26;
    name.push_back(c);
  }

  return name;
}

// Функция генерации значения от 0 до 1
double Percentge_Gen() {
  static std::mt19937 gen1(std::random_device{}());
  double percentage =  std::uniform_real_distribution<>(0.0, 1.0)(gen1);

  return percentage;
}

// Функция генерации имени и значения в структуре
void Decl_str(std::vector<Array_of_str>& arr) {
  for (size_t i = 0; i < arr.size(); ++i) {
    arr[i].name = Name_Gen();
    arr[i].percentage = Percentge_Gen();
  }
}

// Булева функция для сортировки массива
bool Bool_sort(const Array_of_str arr1, const Array_of_str arr2) {
  return arr1.percentage < arr2.percentage;
}

// Функция сортировки массива
void Sort_Arr(std::vector<Array_of_str>& arr) {
  std::sort(arr.begin(), arr.end(), Bool_sort);
}

// Функция определения времени работы программы
double Sorting_Timer(std::vector<Array_of_str>& arr, int seed) {
  std::mt19937 gen2(seed);
  Decl_str(arr);

  auto startTime = std::chrono::high_resolution_clock::now();
  Sort_Arr(arr);
  auto endTime = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> Time_res = endTime - startTime;
  return Time_res.count();
}

// Основное тело программы
int main(int argc, char* argv[]) {
  // Массивы сидов и размеров массива структур
  std::vector<int> seeds = { 123, 456, 789 };
  std::vector<int> arr_sizes = { 5000, 10000, 15000, 20000, 25000, 30000, 35000 };

  std::cout << "\t\t\tSorting time, sec\t\t\t" << std::endl;
  std::cout << "---------+------------------------------------------------+------------+\n";
  std::cout << "Size" << "\t |\t\t\t" << "Seed" << "\t\t\t  |   Average  |" << std::endl;
  std::cout << "---------+------------------------------------------------+------------+\n";

  for (auto arr_size : arr_sizes) {
    std::vector<Array_of_str> arr(arr_size);
    double All_Time = 0.0;
    std::cout << arr_size << "\t |";

    for (auto seed : seeds) {
	  // Инициализация массива структур и времени работы программы
      double sorting_time = Sorting_Timer(arr, seed);
      std::cout << "\t" << std::setprecision(2) << std::fixed << sorting_time << "\t  |";
      All_Time += sorting_time;
    }

    double average_time = All_Time / 3;
    std::cout << "    " << std::setprecision(2) << std::fixed << average_time << "    |" << std::endl;
    std::cout << "---------+----------------+---------------+---------------+------------+\n";
  }

  return 0;
}