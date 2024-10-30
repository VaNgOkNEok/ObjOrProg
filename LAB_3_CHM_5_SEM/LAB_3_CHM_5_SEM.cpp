#include <iostream>
#include <vector>
#include <cmath>
#include <fstream> 
#include "function.h"

int main() {
    setlocale(LC_ALL, "Russian");

    double smooth_factor = 0.99;
    SmoothSpline my_spline(smooth_factor);

    std::vector<Coordinate> coords;
    for (int i = 0; i < 1043; i++) {
        coords.push_back(Coordinate(i));
    }

    std::ifstream inputFile("text.txt"); // Открываем файл для чтения
    if (!inputFile) { // Проверка успешного открытия файла
        std::cerr << "Не удалось открыть файл!" << std::endl;
        return 1; // Завершаем программу с ошибкой
    }

    std::vector<double> function_values;
    double number;

    // Считываем числа из файла
    while (inputFile >> number) {
        function_values.push_back(number); // Добавляем прочитанное число в вектор
    }

    inputFile.close(); // Закрываем файл


    my_spline.fitSpline(coords, function_values);
    // Создаем объект для записи в файл
    std::ofstream outputFile("output_numbers.txt");


    double spline_result[1043];
    for (int i = 0; i < 1043; i++) {
        try {
            my_spline.evaluateSpline(coords[i], &spline_result[i]);
            outputFile << spline_result[i] << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << std::endl;
        }
    }

    // Проверяем, открывался ли файл успешно
    if (!outputFile) {
        std::cerr << "Не удается открыть файл для записи!" << std::endl;
        return 1; // Возвращаем код ошибки
    }

    // Закрываем файл
    outputFile.close();

    return 0;
}
