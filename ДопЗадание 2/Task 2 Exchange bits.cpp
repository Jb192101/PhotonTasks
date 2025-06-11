// Task 2 Exchange bits.cpp
// Дата выполнения: 3 декабря 2024

// ПОЯСНИТЕЛЬНЫЕ МОМЕНТЫ:
// ~ - побитовая инверсия
// >>, << - побитовый сдвиг вправо и влево (слева направо и наоборот)
// & - побитовое И
// | - побитовое ИЛИ
// ^ - побитовое исключащее ИЛИ

// В числе x младшие m бит заменить на старшие m бит из y, потом инвертировать младшие n бит в числе y
#include <iostream>

std::string toBinary(__int64 n) {
    if (n == 0) return "0";

    std::string binaryString = "";
    while (n > 0) {
        binaryString = (n % 2 == 0 ? "0" : "1") + binaryString;
        n /= 2;
    }
    return binaryString;
}

int main() {
    setlocale(LC_ALL, "RU");

    __int64 x, y, m, n;
    std::cin >> x >> y >> m >> n;

    std::cout << "Исходные данные:" << std::endl;
    std::cout << "x = " << x << std::endl;
    std::cout << "y = " << y << std::endl;
    std::cout << "m = " << m << std::endl;
    std::cout << "n = " << n << std::endl;


    // Замена младших m бит в x на старшие m бит из y
    __int64 mask_x = (~((1LL << m) - 1));
    __int64 mask_y = ((1LL << m) - 1) << (64 - m);

    x = (x & mask_x) | ((y & mask_y) >> (64 - m));


    // Инвертирование n младших бит в y
    y ^= ((1LL << n) - 1);

    std::cout << "\nИзмененные данные:" << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "| Переменная |  Двоичное представление  |" << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "|     x      |     " << toBinary(x) << "          |" << std::endl;
    std::cout << "|     y      |     " << toBinary(y) << "         |" << std::endl;
    std::cout << "-----------------------------------------" << std::endl;

    return 0;
}