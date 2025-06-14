﻿ВВЕДЕНИЕ
	Яркостное преобразование изображения (фильтрация изображения) - операция над изображением, при котором меняются яркости пикселей изображения.
	Яркостное преобразование изображения представляет собой попиксельную операцию, которая устанавливает соответствие между яркостью пикселя результирующего изображения с координатами (x;y) и множеством яркостей пикселей исходного изображения, являющихся соседями пикселя с координатами (x;y) в некоторой, заранее известной, окрестности.
	При яркостных преобразованиях чаще всего изображения рассматриваются как двумерный массив чисел, представляющих яркости пикселей.
	Для многоканальных изображений фильтрация выполняется независимо для каждого канала.
	Классическими примерами яркостных преобразований являются алгоритмы сглаживания (размытия) - срединный фильтр, медианный фильтр, фильтр Гаусса и пр.
	
	Срединный фильтр [размера k] - яркостное преобразование, в котором в качестве яркости пикселя результирующего изображения с координатами (x;y) используется среднее арифметическое яркостей пикселей исходного изображения, находящихся в окрестности kxk вокруг пикселя исходного изображения с координатами (x;y). Т.е. в этом фильтре вычисляется мат. ожидание яркостей пикселей, находящихся в окрестности пикселя с заданными координатами:
	
	I'(X,Y) = Round[Average({I(X+i,Y+j)}], i = -k/2..k/2, j = -k/2..k/2, где
	I'(X,Y)			- яркость результирующего пикселя;
	{I(X+i,Y+j)}	- множество яркостей исходного пикселя.
	
	Размер окрестности k также часто называется размером окна фильтрации (k >= 1 и всегда нечётный), а саму фильтрацию - оконной фильтрацией.
	В общем случае, размер окрестности не обязательно должен быть квадратным или прямоугольным; также, в зависимости от алгоритма, не всегда в фильтрации должны участвовать все пиксели окрестности.
	
	При использованием окна фильтрации имеется следующая проблема: при выходе окна фильтрации за пределы изображения часть пикселей имеет неизвестные яркости и фильтрации не может быть выполнена корректно.
	Данная проблема проявляется на краях изображения и, в общем случае, не имеет решения.
	Частное решение этой проблемы заключается в дополнении изображения на краях (padding) некоторыми "особыми" пикселями. Существуют следующие основные способы дополнения изображения:
1. Дублирование яркостей крайних строк и столбцов;
2. Зеркальное отражение строк и столбцов.
	
	Исходное изображение - левый верхний угол заполнен числами 1..25:
         0           4
      *----------------> X
    0 |  1  2  3  4  5
      |  6  7  8  9 10
      | 11 12 13 14 15
      | 16 17 18 19 20
    4 | 21 22 23 24 25
      |                ...
      V
      Y
	
	Дублирование яркостей (для k = 5):
  1  1    1  2  3  4  5
  1  1    1  2  3  4  5
       *----------------> X
  1  1 |  1  2  3  4  5
  6  6 |  6  7  8  9 10
 11 11 | 11 12 13 14 15
 16 16 | 16 17 18 19 20
 21 21 | 21 22 23 24 25
       |                ...
       V
       Y	
	
	Зеркальное отражение яркостей (для k = 5):
 13 12   11 12 13 14 15
  8  7    6  7  8  9 10 ...
       *----------------> X
  3  2 |  1  2  3  4  5
  8  7 |  6  7  8  9 10
 13 12 | 11 12 13 14 15
 18 17 | 16 17 18 19 20
 23 22 | 21 22 23 24 25
 ...   |                ...
       V
       Y	
	
	При зеркалировании может возникнуть проблема нехватки пикселей в ситуации, когда k/2 меньше размера изображения. В такой ситуации используется повторное зеркалирование - зеркалированная строка повторно отражается ещё раз. Пример (размеры изображения - 3x3; k = 9):
	
  1  2  3  2    1  2  3    2  1  2  3	
  6  7  8  7    6  7  8    7  6  7  8
 11 12 13 12   11 12 13   12 11 12 13
  6  7  8  7    6  7  8    7  6  7  8
             *-----------> X
  1  2  3  2 |  1  2  3 |  2  1  2  3
  6  7  8  7 |  6  7  8 |  7  6  7  8
 11 12 13 12 | 11 12 13 | 12 11 12 13
             |----------*
  6  7  8  7 V  6  7  8    7  6  7  8
  1  2  3  2 Y  1  2  3    2  1  2  3
  6  7  8  7    6  7  8    7  6  7  8
 11 12 13 12   11 12 13   12 11 12 13 


ЗАДАНИЕ
	Написать консольное приложение на языке программирования C++, выполняющее следующие задачи:
1. Открытие исходного изображения BMP (изображения в оттенках серого и цветные изображения);
2. Выполнить срединную фильтрацию исходного изображения для следующих алгоритмов дополнения изображения:
	а) дополнение изображения нулями;
	б) дополнение изображения крайними строками/столбцами;
	в) зеркальное отражение;
 размеры окна фильтрации k задаются пользователем при запуске программы;
 итого, выполняется 3 фильтрации, т.е. получается 3 результирующих изображения;
3. Замеры времени отдельно для каждой фильтрации и вывод времён в консоль (в секундах с точностью 3 знака после запятой);
4. Сохранение фильтрованных изображений в формате BMP; имена результирующих изображений должны содержать названия алгоритмов дополнения, чтобы их можно было отличать друг от друга.

РЕЗУЛЬТАТ ВЫПОЛНЕНИЯ ЗАДАНИЯ
	Задание считается выполненным, если программа выполняет поставленные перед ней задачи.

ПРИМЕЧАНИЯ
1. Продолжение зеркального отражения программировать не нужно (k/2 всегда должен быть меньше размеров изображения).
2. Алгоритм дополнения изображения может быть реализован двумя основными способами:
	а) Выделение под растр входного изображения большего размера памяти (чтобы умещались дополненные строки и столбцы), чтение растра в середину выделенной памяти, дополнение изображения и фильтрация пикселей растра;
	б) Фильтрация пикселей одновременно с проверкой выхода окна фильтрации за пределы изображения и с дополнением, если это необходимо.
	Способ а) требует больше памяти, однако, в общем случае (при размере фильтра много меньшем размеров изображения), размер дополнительной памяти много меньше размеров изображения. К тому же, при использовании этого способа не усложняется код сглаживающего фильтра. По этим причинам рекомендуется использовать именно этот способ дополнения изображения.
3. Дополнение изображения и фильтрация должны выполняться после полной загрузки изображения из файла в оперативную память; сохранение результирующего изображения в файл должно выполняться после завершения фильтрации.
