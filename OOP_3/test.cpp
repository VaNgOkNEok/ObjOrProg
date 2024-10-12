#define _USE_MATH_DEFINES //включает константы, такие как M_PI
#include <cmath>

#include <iostream>
#include <cstdio>
#include <stdexcept> // Для std::runtime_error
#include "Primary.h" // Подключаем ваш класс Primary


void testPrimaryClass() {
    Primary primary;
    float* array = new float[4];

    float distribution_function;
    float f;
    float K;
    float P;

    float array_of_shape_ν[]{ 0.5, 0.75, 1, 1.5, 2, 2.5, 3 };
    float array_of_disp[]{ 8.08, 3.71, 2.24, 1.31, 1.08, 1.02, 1.00 };
    float array_of_exc[]{ 2.94, 2.75, 2.37, 1.30, 0.51, 0.16, 0.04 };
    float array_of_P[]{ 0.214, 0.405, 0.585, 0.834, 0.946, 0.986, 0.997 };
    float array_of_f0[]{ 0.223, 0.296, 0.342, 0.384, 0.396, 0.398, 0.399 };

    for (int i = 0; i < 7; i++)
    try {
        primary.setshape_ν(array_of_shape_ν[i]);
        array = primary.distribution_parameters(primary.getshape_ν(), primary.getshift_μ(), primary.getscale_λ());

        if (abs(array[1] - array_of_disp[i]) > 0.01) {
            throw 0;
        }

        if (abs(array[3]-array_of_exc[i]) > 0.01) {
            throw 0;
        }

        if (abs(primary.distribution_density(0, primary.getshape_ν(), primary.getshift_μ(), primary.getscale_λ()) - array_of_f0[i]) > 0.001) {
            throw 0;
        }

        distribution_function = 0.5 * (1 + erf(primary.getshape_ν() / sqrt(2)));
        f = exp(-0.5 * primary.getshape_ν() * primary.getshape_ν()) / sqrt(2 * M_PI);
        K = (2 / primary.getshape_ν()) * f + 2 * distribution_function - 1;
        P = (2 * distribution_function - 1) / K;

        if (abs(P - array_of_P[i]) > 0.001) {
            throw 0;
        }
    }
    catch (int error) {
        if (error == 0) {
            std::cout << "Ошибка при форме равной " << primary.getshape_ν() << std::endl;
        }
    }

    
}
