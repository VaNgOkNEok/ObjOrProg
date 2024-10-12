#include "Mixture.h"
#include <cmath>
#include <iostream>

// Возвращает компонент 1
Primary& Mixture::component1() { return d1; }

// Возвращает компонент 2
Primary& Mixture::component2() { return d2; }

// Возвращает вероятность p
float Mixture::getP() { return p; }

// Вычисление плотности распределения смеси двух компонент
float Mixture::distribution_density_2(const float x)
{
    float f;
    f = (1 - p) * d1.distribution_density(x, d1.getshape_ν(), d1.getshift_μ(), d1.getscale_λ())
        + p * d2.distribution_density(x, d2.getshape_ν(), d2.getshift_μ(), d2.getscale_λ());
    return f;
}

// Возвращает параметры распределения смеси
float* Mixture::distribution_parameters_2()
{
    // Массив для хранения параметров: среднее, дисперсия, асимметрия, эксцесс
    float* arr = new float[4];

    // Получаем параметры распределений компонентов
    auto d1_params = d1.distribution_parameters(d1.getshape_ν(), d1.getshift_μ(), d1.getscale_λ());
    auto d2_params = d2.distribution_parameters(d2.getshape_ν(), d2.getshift_μ(), d2.getscale_λ());

    // Среднее и дисперсия для каждого компонента
    float mean1 = d1_params[0];
    float mean2 = d2_params[0];
    float var1 = d1_params[1];
    float var2 = d2_params[1];

    // Вычисление параметров для смеси
    arr[0] = (1 - p) * mean1 + p * mean2; // Среднее
    arr[1] = (1 - p) * (std::pow(mean1, 2) + var1) + p * (std::pow(mean2, 2) + var2) - std::pow(arr[0], 2); // Дисперсия

    // Асимметрия
    arr[2] = ((1 - p) * (std::pow(mean1 - arr[0], 3) + 3 * (mean1 - arr[0]) * var1) +
        p * (std::pow(mean2 - arr[0], 3) + 3 * (mean2 - arr[0]) * var2)) / std::pow(arr[1], 1.5);

    // Эксцесс
    arr[3] = ((1 - p) * (std::pow(mean1 - arr[0], 4) + 6 * (mean1 - arr[0]) * var1 + 3 * std::pow(var1, 2)) +
        p * (std::pow(mean2 - arr[0], 4) + 6 * (mean2 - arr[0]) * var2 + 3 * std::pow(var2, 2))) / std::pow(arr[1], 2);

    return arr;
}

float* Mixture::random_x_mixture(const int size) {
    float* X = new float[size];
    float r;
    for (int i = 0; i < size; i++) {
        // Генерируем случайную величину r
        r = d1.random_variable_r();

        // Определяем из какой компоненты брать случайное значение
        if (r > p) {
            X[i] = d1.random_variable_x(d1.getshape_ν(), d1.getshift_μ(), d1.getscale_λ());
        }
        else {
            X[i] = d2.random_variable_x(d2.getshape_ν(), d2.getshift_μ(), d2.getscale_λ());
        }
    }
    return X;
}
float* Mixture::random_f_mixture(float* X, const int size) {
    float* f = new float[size];
    for (int i = 0; i < size; i++)
        f[i] = distribution_density_2(X[i]);
    return f;
}

// Генерация массива случайных величин для смеси
void Mixture::random_number_mixture(int size) {
    // Выделение памяти под массивы X и f
    float* X = random_x_mixture(size);
    float* f = random_f_mixture(X, size);
    std::ofstream out;          // поток для записи
    out.open("output2.txt");
    for (int i = 0; i < size; i++) {
        out << X[i] << " " << f[i] << std::endl;
    }
    out.close();
}

// Сохранение параметров смеси в файл
void Mixture::save_mix(FILE* stream) const{
    // Проверка, что указатель на файл не равен nullptr
    if (stream != nullptr) {
        // Сохраняем параметры компонентов и вероятность p
        d1.save(stream);
        d2.save(stream);
        fprintf(stream, "%.2f\n", p);
    }
    else {
        std::cerr << "Ошибка: Указатель на файл равен nullptr." << std::endl;
    }
}

// Загрузка параметров смеси из файла
void Mixture::load_mix(FILE* stream) {
    // Проверка, что указатель на файл не равен nullptr
    if (stream != nullptr) {
        // Загружаем параметры компонентов и вероятность p
        d1.load(stream);
        d2.load(stream);
        fscanf_s(stream, "%lf\n", &p);
    }
    else {
        std::cerr << "Ошибка: Указатель на файл равен nullptr." << std::endl;
    }
}
