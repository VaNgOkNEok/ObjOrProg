#pragma once
#define _USE_MATH_DEFINES //включает константы, такие как M_PI
#include <cmath>
#include <fstream>
#include  <iostream>
// только описания

class Primary {
private:

	float shift_μ = 0; // параметр сдвига
	float scale_λ = 1; // параметр масштаба
	float shape_ν = 1; // параметр формы

public:
	void setshape_ν(float newShift);  // изменяет параметр формы
	void setshift_μ(float newShift);  // изменяет параметр сдвига 
	bool setscale_λ(float newShift);  // изменяет параметр масштаба 

	float getshape_ν() const;  // возвращает значение параметра формы
	float getshift_μ() const;  // возвращает значение параметра сдвига 
	float getscale_λ() const;  // возвращает значение параметра масштаба

	float random_variable_r();
	float emp_random_variable_x();
	float random_variable_x(const float shape_ν, const float shift_μ, const float scale_λ);

	float distribution_density(const float x, const float shape_ν, const float shift_μ, const float scale_λ);

	float* distribution_parameters(const float shape_ν, const float shift_μ, const float scale_λ);

	float get_expectation_by_dataset(const float* dataset_begin, const float* dataset_end);

	float get_dispersion_by_dataset(const float* dataset_begin, const float* dataset_end);

	float get_excess_by_dataset(const float* dataset_begin, const float* dataset_end);

	float get_asymmetry_by_dataset(const float* dataset_begin, const float* dataset_end);

	float get_density_by_dataset(const float x, float* dataset_begin, float* dataset_end);

	float* distribution_parameters_3(const float* dataset_begin, const float* dataset_end);

	void save(FILE* stream) const; // сохранить атрибуты в файл 

	void load(FILE* stream); // загрузить атрибуты из файла
};