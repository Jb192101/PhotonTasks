﻿ВВЕДЕНИЕ
	Медиана - средний элемент в последовательности, отсортированной по возрастанию.
	Медианный фильтр - яркостное преобразование, в котором в качестве яркости пикселя результирующего изображения выступает медиана, вычисленная по пикселям окрестности соответствующего пикселя:
	
	I'(X,Y) = Median({I(X+i,Y+j)}), i = -k/2..k/2, j = -k/2..k/2.
	
	Отличительное особенностью данного фильтра является то, что в качестве результата фильтрации используются только пиксели исходного изображения. Это свойство позволяет использовать данный фильтр для фильтрации резких выбросов яркости на изображении, которые могут быть вызваны различными проблемами:
1. Ошибки в работе других алгоритмов;
2. Ошибки съёмки изображения (испорченная светочувствительная матрица съёмочного устройства);
3. Ошибки передачи информации (импульсные помехи при искажении сигнала в радиоэфире, битая карта памяти и пр.).
	
	Для многоканальных изображений медианная фильтрация выполняется отдельно по каждому каналу.
	
ЗАДАНИЕ
	Написать консольное приложение на языке программирования C++ (или модифицировать любое из предыдущих приложений), выполняющее следующие задачи:
1. Открытие исходного изображения BMP (изображения в оттенках серого и цветные изображения);
2. Выполнить медианную фильтрацию исходного изображения с дополнением изображения методом зеркального отражения двумя способами:
	а) реализацией медианного алгоритма с помощью std::sort;
	б) реализацией медианного алгоритма с помощью std::nth_element;
размеры окна фильтрации k задаются пользователем при запуске программы;
3. Замеры времени фильтрации обоих реализаций и вывод времён в консоль (в секундах с точностью 3 знака после запятой);
4. Сохранение фильтрованных изображений в формате BMP; имена результирующих изображений должны содержать названия реализаций медианного алгоритма, чтобы их можно было отличать друг от друга.

РЕЗУЛЬТАТ ВЫПОЛНЕНИЯ ЗАДАНИЯ
	Задание считается выполненным, если программа выполняет поставленные перед ней задачи.

ПРИМЕЧАНИЯ
	Аналогично предыдущему заданию.
1. Имитировать помехи для их устранения медианным фильтром можно в графических редакторах:
	а) Photoshop: "Фильтр->Шум->Добавить шум...";
	б) GIMP: "Фильтры->Шум->Случайный выбор...".