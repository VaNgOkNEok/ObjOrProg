#include "Empiric.h"
#include <cstdio>

void Empiric::setN(int n0) {
	if (n0 > 1) n = n0;
	else std::cout << "Error\n";
}
void Empiric::setK(int k0) {
	if (k0 > 1) k = k0;
	else std::cout << "Error\n";
}

void Empiric::setData(float* data) {
	if (data != NULL) this->data = data;
	else std::cout << "Error\n";
}
void Empiric::setFr(float* fr) {
	if (fr != NULL) this->fr = fr;
	else std::cout << "Error\n";
}

Empiric::Empiric(int n0, Primary& prima, int k0) :
	n(n0 > 1 ? n0 : throw 2),
	k(k0 > 1 ? k0 : int(log(float(n)) / log(2.)) + 1),
	data(new float[n]), fr(new float[n]) {

	for (int i = 0; i < n; i++) {
		data[i] = prima.emp_random_variable_x();
	}

	// Поиск min и max
	min = max = data[0];
	for (size_t i = 1; i < n; ++i) {
		if (data[i] < min) min = data[i];
		if (data[i] > max) max = data[i];
	}

	fr = obtain_values_density_empirical_function();
}

Empiric::Empiric(int n0, Mixture& mixt, int k0) :
	n(n0 > 1 ? n0 : throw 2),
	k(k0 > 1 ? k0 : int(log(float(n)) / log(2.)) + 1),
	data(new float[n]), fr(new float[n]) {

	data = mixt.random_x_mixture(n);
	fr = mixt.random_f_mixture(data, n);

	min = max = data[0];
	for (size_t i = 1; i < n; ++i) {
		if (data[i] < min) min = data[i];
		if (data[i] > max) max = data[i];
	}
}

Empiric::Empiric(int n0, Empiric& emp, int k0) :
	n(n0 > 1 ? n0 : throw 2),
	k(k0 > 1 ? k0 : int(log(float(n)) / log(2.)) + 1),
	data(new float[n]), fr(new float[n]) {

	Primary prim;
	for (int i = 0; i < n; ++i) {
		data[i] = prim.random_variable_r();
	}

	fr = obtain_values_density_empirical_function();
}

Empiric::~Empiric() {
	delete[] data;
	delete[] fr;
}

Empiric& Empiric::operator=(const Empiric& emp) {
	if (this == &emp) return *this; // Проверка на самоприсваивание

	// Перераспределение памяти при изменении размера
	if (n != emp.n) {
		delete[] data;
		n = emp.n;
		data = new float[n];
	}
	if (k != emp.k) {
		delete[] fr;
		k = emp.k;
		fr = new float[n];
	}

	// Копирование данных
	for (int i = 0; i < n; ++i) {
		data[i] = emp.data[i];
	}
	for (int i = 0; i < n; ++i) {
		fr[i] = emp.fr[i];
	}

	return *this;
}

float Empiric::get_density_by_dataset(const float x) {
	float* dataset_begin = data;
	float* dataset_end = dataset_begin + n;
	int len_of_dataset = dataset_end - dataset_begin;
	min = *dataset_begin;
	max = *dataset_begin;
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

void Empiric::get_array_frequencies_Ni_elements(float*& ni_begin, float*& ni_end) {
	float* dataset_begin = data;
	float* dataset_end = dataset_begin + n;

	min = max = dataset_begin[0];
	for (size_t i = 1; i < n; ++i) {
		if (dataset_begin[i] < min) min = dataset_begin[i];
		if (dataset_begin[i] > max) max = dataset_begin[i];
	}

	float range = max - min;
	float h = range / k;
	float x_first = min;

	// Инициализация массива для частот
	ni_begin = new float[k + 2](); // Инициализация нулями
	ni_begin[k] = x_first;
	ni_begin[k + 1] = h;

	// Подсчет частот
	for (int j = 1; j <= k; ++j) {
		float x_left = x_first + h * (j - 1);
		float x_right = x_first + h * j;

		for (float* temp = dataset_begin; temp < dataset_end; ++temp) {
			if (*temp >= x_left && *temp < x_right) {
				ni_begin[j - 1]++;
			}
			// Для последнего интервала
			if (j == k && *temp == max) {
				ni_begin[j - 1]++;
			}
		}
	}
	ni_end = ni_begin + k + 2;
}

float* Empiric::obtain_values_density_empirical_function() {
	float* dataset_begin = data;
	float* dataset_end = dataset_begin + n;

	float* ni_begin = nullptr;
	float* ni_end = nullptr;
	get_array_frequencies_Ni_elements(ni_begin, ni_end);

	float h = ni_begin[k + 1];
	float x_first = ni_begin[k];

	// Новый массив для плотностей
	float* new_samples = new float[n];
	int j = 0;

	for (float* it = dataset_begin; it < dataset_end; ++it) {
		for (int i = 1; i <= k; ++i) {
			float interval_start = x_first + (i - 1) * h;
			float interval_end = x_first + i * h;

			if (*it >= interval_start && *it < interval_end) {
				new_samples[j] = ni_begin[i - 1] / (n * h);
				j++;
				break;
			}

			if (i == k && *it == interval_end) {
				new_samples[j] = ni_begin[i - 1] / (n * h);
				j++;
				break;
			}
		}
	}

	delete[] ni_begin;
	return new_samples;
}

float Empiric::get_expectation_by_dataset(const float* dataset_begin, const float* dataset_end) {
	float sum = 0;
	int counter = 0;
	for (auto it = dataset_begin; it != dataset_end; ++it) {
		sum += *it;
		counter++;
	}
	return sum / counter;
}

float Empiric::get_dispersion_by_dataset(const float* dataset_begin, const float* dataset_end) {
	float expectation = get_expectation_by_dataset(dataset_begin, dataset_end);
	float sum = 0;
	int counter = 0;
	for (auto it = dataset_begin; it != dataset_end; ++it) {
		sum += pow(*it - expectation, 2);
		counter++;
	}
	return sum / counter;
}

float Empiric::get_excess_by_dataset(const float* dataset_begin, const float* dataset_end) {
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

float Empiric::get_asymmetry_by_dataset(const float* dataset_begin, const float* dataset_end) {
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

float* Empiric::distribution_parameters_3()
{
	const float* dataset_begin = data;
	const float* dataset_end = data + n;
	float* arr = new float[4];
	arr[0] = get_expectation_by_dataset(dataset_begin, dataset_end);
	arr[1] = get_dispersion_by_dataset(dataset_begin, dataset_end);
	arr[2] = get_excess_by_dataset(dataset_begin, dataset_end);
	arr[3] = get_asymmetry_by_dataset(dataset_begin, dataset_end);
	return arr;
}

// Функция для сохранения данных в файл
void Empiric::save_empiric(FILE* stream) const {
	if (stream != nullptr) {
		// Сохранение количества элементов и количество интервалов
		fprintf(stream, "%d\n", n);
		fprintf(stream, "%d\n", k);
		// Сохранение минимального и максимального значений
		fprintf(stream, "%.2f\n", min);
		fprintf(stream, "%.2f\n", max);
		// Сохранение данных массива
		for (int i = 0; i < n; ++i) {
			fprintf(stream, "%.2f\n", data[i]);
		}
		// Сохранение частотной информации
		for (int i = 0; i < n; ++i) {
			fprintf(stream, "%.2f\n", fr[i]);
		}
	}
	else {
		std::cerr << "Ошибка: Указатель на файл равен нулю." << std::endl;
	}
}

// Функция для загрузки данных из файла
void Empiric::load_empiric(FILE* stream) {
	if (stream != nullptr) {
		// Загрузка количества элементов и количество интервалов
		fscanf_s(stream, "%d\n", &n);
		fscanf_s(stream, "%d\n", &k);

		// Загрузка минимального и максимального значений
		fscanf_s(stream, "%f\n", &min);
		fscanf_s(stream, "%f\n", &max);

		// Освобождаем память перед загрузкой новых данных
		delete[] data;
		delete[] fr;

		// Выделяем новую память для данных
		data = new float[n];
		fr = new float[n];

		// Загрузка данных массива
		for (int i = 0; i < n; ++i) {
			fscanf_s(stream, "%f\n", &data[i]);
		}

		// Загрузка частотной информации
		for (int i = 0; i < n; ++i) {
			fscanf_s(stream, "%f\n", &fr[i]);
		}
	}
	else {
		std::cerr << "Ошибка: Указатель на файл равен нулю." << std::endl;
	}
}