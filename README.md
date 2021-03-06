# Японский кроссворд
Японский кроссворд это головоломка, в которой с помощью цифр зашифровано некоторое изображение. Цель - восстановление этого изображения.

# Решатель
Это оконное приложение, использующие Windows API.
Имеется возможность задавать размеры поля и условие кроссворда.
Поддерживается ввод/вывод из файлов.
Также есть возможность редактирования поля кроссворда.

# Алгоритм
Алгоритм решения состоит из нескольких эвристик. Сложность в худшем случае O(n^6). На реальных данных за 2 секунды может решать кроссворды до 100x100.

# Эвристика 1
По заданной строке найти наиболее левые и наиболее правые корректные позиции блоков. Заполнить “перекрытие” наиболее левой и наиболее правой позиций блоков. Пометить пустыми клетки, не относящиеся не к одному из блоков.

# Эвристика 2
Для каждой клетки найти минимальный размер блока, к которому она может относиться. Найти ближайшую пустую клетку слева. Если расстояние между клетками меньше длины блока, заполнить клетки с другой стороны.
