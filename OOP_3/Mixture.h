#include <iostream>
#include <stdexcept> // Для исключений
#include "Primary.h"

class Mixture {
private:
    float p;         // Вероятность для смеси
    Primary d1, d2;  // Две компоненты смеси

public:
    // Конструктор с инициализацией и проверкой вероятности p
    Mixture(Primary& prim1, Primary& prim2, float p0) :
        p(p0 > 0 && p0 < 1 ? p0 : throw std::invalid_argument("p должно быть в диапазоне (0,1)")), d1(prim1), d2(prim2) {}

    // Возвращает первую компоненту
    Primary& component1();

    // Возвращает вторую компоненту
    Primary& component2();

    // Сеттер для компоненты 1
    void setComponent1(Primary& prim1) { d1 = prim1; }

    // Сеттер для компоненты 2
    void setComponent2(Primary& prim2) { d2 = prim2; }

    // Сеттер для вероятности p
    void setP(float p0) {
        if (p0 > 0 && p0 < 1) {
            p = p0;
        }
        else {
            throw std::invalid_argument("p должно быть в диапазоне (0,1)");
        }
    }

    // Возвращает значение p
    float getP();

    // Вычисляет плотность распределения смеси
    float distribution_density_2(const float x);

    // Возвращает параметры распределения смеси
    float* distribution_parameters_2();

    float* random_x_mixture(const int size);
    float* random_f_mixture(float* x, const int size);

    // Генерирует массив случайных величин для смеси
    void random_number_mixture(int size);

    // Сохраняет параметры смеси в файл
    void save_mix(FILE* stream) const;

    // Загружает параметры смеси из файла
    void load_mix(FILE* stream);
};