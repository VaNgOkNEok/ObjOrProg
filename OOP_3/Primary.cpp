#include "Primary.h"

void Primary::setshift_μ(float newShift) // изменяет параметр сдвига 
{
	shift_μ = newShift;
}
float Primary::getshift_μ() const// возвращает значение параметра сдвига 
{
	return shift_μ;;
}

bool Primary::setscale_λ(float newScale)  // изменяет параметр масштаба 
{
	scale_λ = newScale;
	try {
		if (newScale == 0) {
			throw std::runtime_error("Некорректный ввод данных."); // Генерируем исключение
		}
		return 1;
	}

	catch (const std::runtime_error& e) {
		return 0; // Обработка исключения
	}
}
float Primary::getscale_λ() const  // возвращает значение параметра масштаба
{
	return scale_λ;
}

void Primary::setshape_ν(float newShape)  // изменяет параметр формы
{
	shape_ν = newShape;
}
float Primary::getshape_ν() const  // возвращает значение параметра формы
{
	return shape_ν;
}

float Primary::random_variable_r()
{
	float r; // получения реализации непрерывной случайной величины, равномерно распределенной на интервале (0, 1)
	do r = (float)rand() / RAND_MAX; while (r == 0. || r == 1.);

	return r;
}

float Primary::random_variable_x(float shape_ν, float shift_μ, float scale_λ)
{
	float r1 = random_variable_r(); // получения реализации непрерывной случайной величины, равномерно распределенной на интервале (0, 1)

	float distribution_function = 0.5 * (1 + erf(shape_ν / sqrt(2)));
	float f = exp(-0.5 * shape_ν * shape_ν) / sqrt(2 * M_PI);
	float K = (2 / shape_ν) * f + 2 * distribution_function - 1;
	float P = (2 * distribution_function - 1) / K;

	if (r1 <= P) //Получить реализацию x1 нормальной случайной величины(методом Бокса–Мюллера)
	{
		float x1;
		float r2, r3;
		do {
			r2 = random_variable_r();
			r3 = random_variable_r();

			x1 = sqrt(-2 * log(r2)) * cos(2 * M_PI * r3);
		} while (x1< (-1 * shape_ν) or x1 > shape_ν);
		return x1 * scale_λ + shift_μ;
	}
	else
	{
		float x2;
		float r4;
		do r4 = (float)rand() / RAND_MAX; while (r4 == 0. || r4 == 1.);

		x2 = shape_ν - log(r4) / shape_ν;
		if (r1 < (1 + P) / 2) return x2 * scale_λ + shift_μ;
		else return -1 * (x2 * scale_λ) + shift_μ;
	}
}

float Primary::emp_random_variable_x()
{
	float r1 = random_variable_r(); // получения реализации непрерывной случайной величины, равномерно распределенной на интервале (0, 1)

	float distribution_function = 0.5 * (1 + erf(shape_ν / sqrt(2)));
	float f = exp(-0.5 * shape_ν * shape_ν) / sqrt(2 * M_PI);
	float K = (2 / shape_ν) * f + 2 * distribution_function - 1;
	float P = (2 * distribution_function - 1) / K;

	if (r1 <= P) //Получить реализацию x1 нормальной случайной величины(методом Бокса–Мюллера)
	{
		float x1;
		float r2, r3;
		do {
			r2 = random_variable_r();
			r3 = random_variable_r();

			x1 = sqrt(-2 * log(r2)) * cos(2 * M_PI * r3);
		} while (x1< (-1 * shape_ν) or x1 > shape_ν);
		return x1 * scale_λ + shift_μ;
	}
	else
	{
		float x2;
		float r4;
		do r4 = (float)rand() / RAND_MAX; while (r4 == 0. || r4 == 1.);

		x2 = shape_ν - log(r4) / shape_ν;
		if (r1 < (1 + P) / 2) return x2 * scale_λ + shift_μ;
		else return -1 * (x2 * scale_λ) + shift_μ;
	}
}

float Primary::distribution_density(const float x, const float shape_ν, const float shift_μ, const float scale_λ)
{
	float distribution_function = 0.5 * (1 + erf(shape_ν / sqrt(2)));
	float f = exp(-0.5 * shape_ν * shape_ν) / sqrt(2 * M_PI);
	float K = (2 / shape_ν) * f + 2 * distribution_function - 1;
	float P = (2 * distribution_function - 1) / K;

	if (abs(x) <= (shape_ν * scale_λ + shift_μ)) return 1 / (sqrt(2 * M_PI) * K * scale_λ) * exp(-(x - shift_μ) * (x - shift_μ) / (2 * scale_λ * scale_λ));
	else return 1 / (sqrt(2 * M_PI) * K * scale_λ) * exp(shape_ν * shape_ν / 2 - shape_ν * abs((x - shift_μ) / scale_λ));
}

float* Primary::distribution_parameters(const float shape_ν, const float shift_μ, const float scale_λ)
{
	float* array = new float[4];

	float distribution_function = 0.5 * (1 + erf(shape_ν / sqrt(2)));
	float f = exp(-0.5 * shape_ν * shape_ν) / sqrt(2 * M_PI);
	float K = (2 / shape_ν) * f + 2 * distribution_function - 1;
	float P = (2 * distribution_function - 1) / K;

	array[0] = 0 * scale_λ + shift_μ;

	array[1] = 1 + (2 * f * (shape_ν * shape_ν + 2)) / (shape_ν * shape_ν * shape_ν * K) * scale_λ * scale_λ;

	array[2] = 0;

	array[3] = (3 * (2 * distribution_function - 1) + 2 * f * (24 / pow(shape_ν, 5) + 24 / pow(shape_ν, 3) + 12 / shape_ν + shape_ν)) / (array[1] * array[1] * K) - 3;

	return array;
}

float Primary::get_expectation_by_dataset(const float* dataset_begin, const float* dataset_end) {
	float sum = 0;
	int counter = 0;
	for (auto it = dataset_begin; it != dataset_end; ++it) {
		sum += *it;
		counter++;
	}
	return sum / counter;
}

float Primary::get_dispersion_by_dataset(const float* dataset_begin, const float* dataset_end) {
	float expectation = get_expectation_by_dataset(dataset_begin, dataset_end);
	float sum = 0;
	int counter = 0;
	for (auto it = dataset_begin; it != dataset_end; ++it) {
		sum += pow(*it - expectation, 2);
		counter++;
	}
	return sum / counter;
}

float Primary::get_excess_by_dataset(const float* dataset_begin, const float* dataset_end) {
	float expectation = get_expectation_by_dataset(dataset_begin, dataset_end);
	float dispersion = get_dispersion_by_dataset(dataset_begin, dataset_end);

	float sum = 0;
	int counter = 0;
	for (auto it = dataset_begin; it != dataset_end; ++it) {
		sum += pow(*it - expectation, 4);
		counter++;
	}
	return (sum / counter) / pow(dispersion, 2) - 3;
}

float Primary::get_asymmetry_by_dataset(const float* dataset_begin, const float* dataset_end) {
	float expectation = get_expectation_by_dataset(dataset_begin, dataset_end);
	float dispersion = get_dispersion_by_dataset(dataset_begin, dataset_end);
	float sum = 0;
	int counter = 0;
	for (auto it = dataset_begin; it != dataset_end; ++it) {
		sum += pow(*it - expectation, 3);
		counter++;
	}
	return (sum / counter) / pow(dispersion, 1.5);
}

float Primary::get_density_by_dataset(const float x, float* dataset_begin, float* dataset_end) {
	int len_of_dataset = dataset_end - dataset_begin;
	float k;
	if (len_of_dataset >= 50) {
		k = 1 + 3.322 * log10(len_of_dataset);
	}
	else {
		k = 8;
	}
	float min = *dataset_begin;
	float max = *dataset_begin;
	float* temp = dataset_begin;

	while (temp < dataset_end) {
		if (*temp < min) {
			min = *temp;
		}
		if (*temp > max) {
			max = *temp;
		}
		temp++;
	}
	float r = max - min;
	float h = r / k;
	float x_first = min - h * 0.5;
	float x_left = x_first + floor((x - x_first) / h) * h;
	float x_right = x_left + h;
	float count = 0;
	temp = dataset_begin;
	for (int i = 0; i < len_of_dataset; i++) {
		if (*temp >= x_left and *temp < x_right) {
			count++;
		}
		temp++;
	}
	return count / len_of_dataset;
}

float* Primary::distribution_parameters_3(const float* dataset_begin, const float* dataset_end)
{
	float* arr = new float[4];
	arr[0] = get_expectation_by_dataset(dataset_begin, dataset_end);
	arr[1] = get_dispersion_by_dataset(dataset_begin, dataset_end);
	arr[2] = get_excess_by_dataset(dataset_begin, dataset_end);
	arr[3] = get_asymmetry_by_dataset(dataset_begin, dataset_end);
	return arr;
}

void Primary::save(FILE* stream) const // сохранить атрибуты в файл 
{
	// Запись значений атрибутов в файл
	if (stream != nullptr) { // Проверяем, что указатель не равен нулю

		fprintf(stream, "%.2f\n", shape_ν);
		fprintf(stream, "%.2f\n", shift_μ);
		fprintf(stream, "%.2f\n", scale_λ);
	}
	else {
		std::cerr << "Ошибка: Указатель на файл равен нулю." << std::endl;
	}
}

void Primary::load(FILE* stream) // загрузить атрибуты из файла
{
	if (stream != nullptr) { // Проверяем, что указатель не равен нулю
		float tempShape, tempShift, tempScale;

		// Загрузка значений атрибутов из файла
		fscanf_s(stream, "%lf\n", &tempShape);
		fscanf_s(stream, "%lf\n", &tempShift);
		fscanf_s(stream, "%lf\n", &tempScale);

		try {
			if (!setscale_λ(tempScale)) { // Проверка, установилось ли значение масштаба
				throw std::runtime_error("Некорректный ввод: масштаб не может быть равен нулю.");
			}
			setshape_ν(tempShape); // Устанавливаем значения через сеттеры
			setshift_μ(tempShift);
		}
		catch (const std::runtime_error& e) {
			std::cerr << "Ошибка: " << e.what() << std::endl; // Выводим сообщение об ошибке
			system("pause"); // Ожидание нажатия клавиши для закрытия консоли
			exit(EXIT_FAILURE); // Завершаем программу
		}
	}
	else {
		std::cerr << "Ошибка: Указатель на файл равен нулю." << std::endl;
	}
}