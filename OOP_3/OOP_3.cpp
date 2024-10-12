#define _USE_MATH_DEFINES //включает константы, такие как M_PI
//#include <iostream>
//#include <cmath>
//#include <fstream>

#include <string>
#include <time.h>
#include "test.h"
#include "Empiric.h" // включить интерфейс
// моделирование случайной величины
//float Primary::randNum() const { . . . }

using namespace std;

bool IsCorrect(string check)
{
	bool hasDecimalPoint = false;

	// Пустая строка недопустима
	if (check.empty()) {
		return false;
	}

	// Проверяем первый символ (может быть цифрой или минусом, но не точкой)
	if (check[0] != '-' && (check[0] < '0' || check[0] > '9')) {
		return false;
	}

	// Проходим по остальным символам
	for (size_t i = 1; i < check.length(); ++i) {
		char ch = check[i];

		if (ch >= '0' && ch <= '9') {
			// Если символ - цифра, все хорошо
			continue;
		}
		else if (ch == '.') {
			// Если точка уже была, то строка недопустима
			if (hasDecimalPoint) {
				return false;
			}
			// Отмечаем, что встретили точку
			hasDecimalPoint = true;
		}
		else {
			// Если встретился недопустимый символ
			return false;
		}
	}

	// Строка допустима, если содержит только допустимые символы
	return true;
}

void Input(Primary& primary)
{
	string check;

	int size = 0;

	cout << "Введите значение формы: v = ";
	cin >> check;
	while (!IsCorrect(check)) {
		cout << "Введены некорректные данные" << endl;
		cout << "Введите значение формы: v = ";
		cin >> check;
	}
	primary.setshape_ν(stod(check));
	cout << endl;

	cout << "Введите значение сдвига: u = ";
	cin >> check;
	while (!IsCorrect(check)) {
		cout << "Введены некорректные данные" << endl;
		cout << "Введите значение сдвига: u = ";
		cin >> check;
	}
	primary.setshift_μ(stod(check));
	cout << endl;

	check = "1";
	do {
		if (primary.setscale_λ(stod(check)) == 0); //cout << "Некорректный ввод данных." << endl;
		cout << "Введите значение масштаба, отличное от нуля: h = ";
		cin >> check;
		while (!IsCorrect(check)) {
			cout << "Введены некорректные данные" << endl;
			cout << "Введите значение масштаба: h = ";
			cin >> check;
		}
		primary.setscale_λ(stod(check));
	} while (primary.getscale_λ() == 0);

}

void Input2(Mixture& mix) {
	float shape_v_1, shift_u_1, scale_h_1 = 0, shape_v_2, shift_u_2, scale_h_2 = 0, p;
	string check;
	Primary prim1 = mix.component1(), prim2 = mix.component2();
	cout << "Введите значение формы для первого распределения: v1 = ";
	cin >> check;
	while (!IsCorrect(check)) {
		cout << "Введены некорректные данные" << endl;
		cout << "Введите значение формы для первого распределения: v1 = ";
		cin >> check;
	}
	prim1.setshape_ν(stof(check));
	cout << endl;

	cout << "Введите значение сдвига для первого распределения: u1 = ";
	cin >> check;
	while (!IsCorrect(check)) {
		cout << "Введены некорректные данные" << endl;
		cout << "Введите значение сдвига для первого распределения: u1 = ";
		cin >> check;
	}
	prim1.setshift_μ(stof(check));
	cout << endl;

	do {
		cout << "Введите значение масштаба для первого распределения, отличное от нуля: h1 = ";
		cin >> check;
		while (!IsCorrect(check)) {
			cout << "Введены некорректные данные" << endl;
			cout << "Введите значение масштаба для первого распределения: h1 = ";
			cin >> check;
		}
		scale_h_1 = stof(check);
		if (scale_h_1 == 0) cout << "Некорректный ввод данных." << endl;
	} while (scale_h_1 == 0);
	prim1.setshift_μ(scale_h_1);

	cout << "Введите значение формы для второго распределения: v2 = ";
	cin >> check;
	while (!IsCorrect(check)) {
		cout << "Введены некорректные данные" << endl;
		cout << "Введите значение формы для второго распределения: v2 = ";
		cin >> check;
	}
	prim2.setshape_ν(stof(check));
	cout << endl;

	cout << "Введите значение сдвига для второго распределения: u2 = ";
	cin >> check;
	while (!IsCorrect(check)) {
		cout << "Введены некорректные данные" << endl;
		cout << "Введите значение сдвига для второго распределения: u2 = ";
		cin >> check;
	}
	prim2.setshift_μ(stof(check));
	cout << endl;

	do {
		cout << "Введите значение масштаба для второго распределения, отличное от нуля: h2 = ";
		cin >> check;
		while (!IsCorrect(check)) {
			cout << "Введены некорректные данные" << endl;
			cout << "Введите значение масштаба для второго распределения: h2 = ";
			cin >> check;
		}
		scale_h_2 = stof(check);
		if (scale_h_2 == 0) cout << "Некорректный ввод данных." << endl;
	} while (scale_h_2 == 0);
	prim2.setshift_μ(scale_h_2);

	do {
		cout << "Введите значение параметра смеси в диапазоне (0, 1): p = ";
		cin >> check;
		while (!IsCorrect(check)) {
			cout << "Введены некорректные данные" << endl;
			cout << "Введите значение параметра смеси в диапазоне (0, 1): p = ";
			cin >> check;
		}
		p = stof(check);
		if (p >= 1 or p <= 0) cout << "Некорректный ввод данных." << endl;
	} while (p >= 1 or p <= 0);
	// Создание и возврат объекта Mixture
	mix.setComponent1(prim1);
	mix.setComponent2(prim2);
	mix.setP(p);
}

int main()
{
	setlocale(LC_ALL, "Russian");
	Primary primary;
	testPrimaryClass(); // Вызов функции тестирования
	srand((unsigned)time(0));
	setlocale(LC_NUMERIC, "eng");
	std::ofstream out;          // поток для записи
	bool In_Data = false, In_Data2 = false;

	FILE* file;
	errno_t err = fopen_s(&file, "output.txt", "r");

	float x = 0;
	float shape_ν_1, shift_μ_1, scale_λ_1, shape_ν_2, shift_μ_2, scale_λ_2;
	string action_number = "";
	string check;
	string distribution_number = "";
	Primary prim1;
	Primary prim2;
	Mixture mixture(prim1, prim2, 0.5);
	Empiric empiricalFunction(10, mixture, 0);

	int size = 0, size_2 = 0;
	float* f_1 = new float[size];
	float* f_2 = new float[size];
	float* r_1 = new float[size];
	float* X_2 = new float[size];
	bool flag_mas_1 = false;
	bool flag_mas_2 = false;
	bool flag_mas_3 = false;

	//Открытие файла для чтения
	if (err == 0) {
		cout << "Введите номер ";
		cin >> action_number;
		cout << endl;
		if (action_number == "1") {
			primary.load(file); // Загружаем данные из файла
		} else if (action_number == "2")
		{
			mixture.load_mix(file);
		}
		else if (action_number == "3")
		{
			Input(primary);
			cout << "Введите количество генерируемых значений: n = ";
			cin >> size;
			while (size < 0)
			{
				cout << "Некоректный ввод данных.";
				cout << "Введите количество генерируемых значений: n = ";
				cin >> size;
			}
			In_Data = true;
			f_1 = new float[size];
			r_1 = new float[size];
			Empiric empiricalFunction(size, primary, 0);
		}
		else if (action_number == "4") {
			Input2(mixture);
			cout << "Введите количество генерируемых значений: n = ";
			cin >> size;
			while (size < 0)
			{
				cout << "Некоректный ввод данных.";
				cout << "Введите количество генерируемых значений: n = ";
				cin >> size;
			}
			In_Data2 = true;
			f_2 = new float[size];
			X_2 = new float[size];
			Empiric empiricalFunction(size, mixture, 0);
		}
		cout << "Данные успешно загружены из output.txt." << endl;
	}
	else {
		cerr << "Ошибка: не удалось открыть файл для чтения." << endl;
	}
	fclose(file); // Закрываем файл

	do {
		do {
			cout << "Выберите номер распределения:" << endl;
			cout << "1. Распределение Хьюбера" << endl;
			cout << "2. Распределение в виде смесей" << endl;
			cout << "3. Эмперическое распределение" << endl;
			cout << "4. Выход" << endl;
			cout << "Введите номер распределения - ";
			cin >> distribution_number; cout << endl;
		} while (distribution_number != "1" and distribution_number != "2" and distribution_number != "3" and distribution_number != "4");

		if (distribution_number == "1")
		{
			if (In_Data == 0) { Input(primary); In_Data = true; }
			bool flag = true;
			do {
				do {
					cout << "Выберите действие из списка:" << endl;
					cout << "1. Вычисление значений плотности распределения по аргументу" << endl;
					cout << "2. Вычисление математического ожидания, дисперсии, коэффициентов асимметрии и эксцесса" << endl;
					cout << "3. Перемоделирование величин" << endl;
					cout << "4. Генерация значений функции плотности" << endl;
					cout << "5. Выход" << endl;
					cout << "Введите номер действия - ";
					cin >> action_number; cout << endl;
				} while (action_number != "1" and action_number != "2" and action_number != "3" and action_number != "4" and action_number != "5");

				if (flag_mas_1 == 1) {
					out.open("output1.txt");      // открываем файл для записи
					int i = 0;
					while (i < size)
					{
						if (out.is_open())
						{
							out << r_1[i] << " " << f_1[i] << endl;
						}
						i++;
					}
					out.close();
				}

				if (action_number == "1") {
					cout << "Введите параметр x: x = ";
					cin >> x;
					cout << endl;
					cout << "Значение плотности распределения = " << primary.distribution_density(x, primary.getshape_ν(), primary.getshift_μ(), primary.getscale_λ()) << endl;
				}

				if (action_number == "2") {
					char nature_of_distribution = '1';
					do {
						if (nature_of_distribution != '1') cout << "Введены некорректные данные." << endl;
						cout << "Выберете какие параметры хотите рассчитать:" << endl;
						cout << "1. Теоритические" << endl;
						cout << "2. Практические" << endl;
						cin >> nature_of_distribution;
					} while (nature_of_distribution != '1' and nature_of_distribution != '2');
					if (nature_of_distribution == '1') {
						float* array = new float[4];
						array = primary.distribution_parameters(primary.getshape_ν(), primary.getshift_μ(), primary.getscale_λ());
						cout << "Математическое ожидание = " << array[0] << endl;
						cout << "Дисперсия = " << array[1] << endl;
						cout << "Коэффициент ассиметрии = " << array[2] << endl;
						cout << "Коэффициент Эксцесса = " << array[3] << endl;
					}
					else {
						if (flag_mas_1 == 1) {
							float* array = new float[4];
							array = primary.distribution_parameters_3(r_1, r_1 + size);
							cout << "Математическое ожидание = " << array[0] << endl;
							cout << "Дисперсия = " << array[1] << endl;
							cout << "Коэффициент ассиметрии = " << array[2] << endl;
							cout << "Коэффициент Эксцесса = " << array[3] << endl;
						}
						else {
							cout << "Введите количество генерируемых значений: n = ";
							int i = 0;
							float r;
							cin >> size;
							while (size < 0)
							{
								cout << "Некоректный ввод данных.";
								cout << "Введите количество генерируемых значений: n = ";
								cin >> size;
							}

							f_1 = new float[size];
							r_1 = new float[size];
							flag_mas_1 = true;

							float* array = new float[4];
							array = primary.distribution_parameters_3(r_1, r_1 + size);
							cout << "Математическое ожидание = " << array[0] << endl;
							cout << "Дисперсия = " << array[1] << endl;
							cout << "Коэффициент ассиметрии = " << array[2] << endl;
							cout << "Коэффициент Эксцесса = " << array[3] << endl;
						}
					}
					float distribution_function = 0.5 * (1 + erf(primary.getshape_ν() / sqrt(2)));
					float f = exp(-0.5 * primary.getshape_ν() * primary.getshape_ν()) / sqrt(2 * M_PI);
					float K = (2 / primary.getshape_ν()) * f + 2 * distribution_function - 1;
					float P = (2 * distribution_function - 1) / K;
					cout << "Вероятность попадания в центральный интервал = " << P << endl;
				}

				if (action_number == "3")
				{
					Input(primary);
					flag_mas_1 = 0;
				}

				if (action_number == "4") {
					char regeneration = '1';
					if (flag_mas_1 == 1) {
						do {
							if (regeneration != '1') cout << "Некоректный ввод данных.";
							cout << "Вы хотите перегенирировать выборку?" << endl;
							cout << "1. Да" << endl;
							cout << "2. Нет" << endl;
							cin >> regeneration;
						} while (regeneration != '1' and regeneration != '2');
						if (regeneration == '1') flag_mas_1 = 0;
					}
					if (flag_mas_1 == 0) {
						cout << "Введите количество генерируемых значений: n = ";
						int i = 0;
						float r;
						cin >> size;
						while (size < 0)
						{
							cout << "Некоректный ввод данных.";
							cout << "Введите количество генерируемых значений: n = ";
							cin >> size;
						}

						f_1 = new float[size];
						r_1 = new float[size];
						flag_mas_1 = true;
					}

					float r;
					int i = 0;
					out.open("output1.txt");      // открываем файл для записи
					while (i < size)
					{
						r = primary.random_variable_x(primary.getshape_ν(), primary.getshift_μ(), primary.getscale_λ());
						r_1[i] = r;
						f_1[i] = primary.distribution_density(r, primary.getshape_ν(), primary.getshift_μ(), primary.getscale_λ());
						if (out.is_open())
						{
							out << r << " " << f_1[i] << endl;
						}
						i++;
					}
					out.close();
				}

				if (action_number == "5") {
					flag = false;
					out.close();
				}

			} while (flag);
		}

		if (distribution_number == "2")
		{
			string action_number = "";
			string check;
			float p = 0;

			if (In_Data2 == false) { // Изменено с 0 на false, так как это флаг логического типа
				Input2(mixture); // Функция для ввода параметров смеси
				In_Data2 = true; // Устанавливаем флаг для предотвращения повторного ввода данных
			}

			do {
				do {
					// Предлагаем пользователю выбрать действие
					cout << "Выберите действие из списка:" << endl;
					cout << "1. Вычисление значений плотности распределения по аргументу" << endl;
					cout << "2. Вычисление математического ожидания, дисперсии, коэффициентов асимметрии и эксцесса" << endl;
					cout << "3. Перемоделирование величин" << endl;
					cout << "4. Генерация значений функции плотности" << endl;
					cout << "5. Выход" << endl;
					cout << "Введите номер действия - ";
					cin >> action_number;
					cout << endl;

				} while (action_number != "1" && action_number != "2" && action_number != "3" && action_number != "4" && action_number != "5");

				if (action_number == "1") {
					// Вычисление плотности по заданному значению x
					float x;
					cout << "Введите значение x = ";
					cin >> x;
					cout << endl;
					float f = mixture.distribution_density_2(x);
					cout << "Значение плотности распределения по аргументу = " << f << endl;
				}

				if (action_number == "2") {
					// Вычисление параметров распределения: математическое ожидание, дисперсия, асимметрия и эксцесс
					cout << "Вычисление математического ожидания, дисперсии, коэффициентов асимметрии и эксцесса" << endl;
					float* array = mixture.distribution_parameters_2();
					cout << "Математическое ожидание = " << array[0] << endl;
					cout << "Дисперсия = " << array[1] << endl;
					cout << "Коэффициент асимметрии = " << array[2] << endl;
					cout << "Коэффициент Эксцесса = " << array[3] << endl;
				}

				if (action_number == "3") {
					// Повторное моделирование смеси
					Input2(mixture); // Пользователь повторно вводит параметры для новой смеси
				}

				if (action_number == "4") {
					// Генерация значений функции плотности
					int size;
					do {
						cout << "Введите объем данных (целым числом больше 0, при введении не целого числа произойдет округление): ";
						cin >> check;
						cout << endl;
						while (!IsCorrect(check)) {
							cout << "Введены некорректные данные. Попробуйте снова." << endl;
							cin >> check;
							cout << endl;
						}
						size = stoi(check);
					} while (size <= 0);

					cout << endl;
					mixture.random_number_mixture(size);
					cout << "Генерация значений функции плотности завершена" << endl;
				}

			} while (action_number != "5"); // Выход из программы
		}

		if (distribution_number == "3")
		{
			string action_number = "";
			float r;
			float p;
			float x, f;
			int i = 0;
			char regeneration = '1';
			do {
				if (regeneration != '1') cout << "Некоректный ввод данных.";
				cout << "Для какого распределения вы хотите получить эмпирическую функцию?" << endl;
				cout << "1. Основное" << endl;
				cout << "2. Смесь" << endl;
				cin >> regeneration;
			} while (regeneration != '1' and regeneration != '2');

			if (regeneration == '1') {
				if (In_Data == 0) {
					Input(primary);
					cout << "Введите количество генерируемых значений: n = ";
					cin >> size;
					while (size < 0)
					{
						cout << "Некоректный ввод данных.";
						cout << "Введите количество генерируемых значений: n = ";
						cin >> size;
					}
					In_Data = true;
				}
				f_1 = new float[size];
				r_1 = new float[size];
				Empiric empiricalFunction(size, primary, 0);
				do {
					if (In_Data == 0) {
						Input(primary);
						cout << "Введите количество генерируемых значений: n = ";
						cin >> size;
						while (size < 0)
						{
							cout << "Некоректный ввод данных.";
							cout << "Введите количество генерируемых значений: n = ";
							cin >> size;
						}
						In_Data = true;
						Empiric empiricalFunction(size, primary, 0);
						delete[] r_1;
						delete[] f_1;
						f_1 = new float[size];
						r_1 = new float[size];
					}
					do {
						cout << "Выберите действие из списка:" << endl;
						cout << "1. Вычисление плотности" << endl;
						cout << "2. Вычисление математического ожидания, дисперсии, коэффициентов асимметрии и эксцесса" << endl;
						cout << "3. Генерация значений функции плотности" << endl;
						cout << "4. Выход" << endl;
						cout << "Введите номер действия - ";
						cin >> action_number; cout << endl;
					} while (action_number != "1" && action_number != "2" && action_number != "3" && action_number != "4");


					if (action_number == "1") {
						cout << "Вычисление значений плотности распределения по аргументу" << endl;
						cout << "Введите значение x = ";
						cin >> x;
						cout << endl;
						cout << "Значение плотности распределения по аргументу = " << empiricalFunction.get_density_by_dataset(x) << endl;
					}

					if (action_number == "2") {
						cout << "Вычисление математического ожидания, дисперсии, коэффициентов асимметрии и эксцесса" << endl;
						float* array = empiricalFunction.distribution_parameters_3(); // используем метод класса
						cout << "Математическое ожидание = " << array[0] << endl;
						cout << "Дисперсия = " << array[1] << endl;
						cout << "Коэффициент ассиметрии = " << array[2] << endl;
						cout << "Коэффициент Эксцесса = " << array[3] << endl;
					}

					if (action_number == "3") {
						cout << "Моделирование случайной величины" << endl;
						regeneration = '1';
						do {
							if (regeneration != '1') cout << "Некоректный ввод данных.";
							cout << "Вы хотите перегенирировать выборку?" << endl;
							cout << "1. Да" << endl;
							cout << "2. Нет" << endl;
							cin >> regeneration;
						} while (regeneration != '1' && regeneration != '2');
						if (regeneration == '1') In_Data = 0;
						if (In_Data == true) {
							do {
								cout << "Выберите способ генерации значений:" << endl;
								cout << "1. Ввод с клавиатуры" << endl;
								cout << "2. Автоматическая генерация" << endl;
								cout << "Введите способ генерации значений - ";
								cin >> regeneration;
								cout << endl;
							} while (regeneration != '1' && regeneration != '2');

							if (regeneration == '1') {
								out.open("output3.txt"); // открываем файл для записи
								for (int i = 0; i < size; i++) {
									cout << "Введите x[" << i << "] = ";
									cin >> r_1[i];
									cout << "Введите f[" << i << "] = ";
									cin >> f_1[i];
									if (out.is_open()) {
										out << r_1[i] << " " << f_1[i] << endl;
									}
								}
								empiricalFunction.setData(r_1);
								empiricalFunction.setFr(f_1);
								out.close();
							}
							else {
								out.open("output3.txt"); // открываем файл для записи
								r_1 = empiricalFunction.getData();
								f_1 = empiricalFunction.getFrequencies(); // используем метод класса
								for (i = 0; i < size; ++i) {
									out << r_1[i] << " " << f_1[i] << endl;
								}
								out.close();
								cout << "Значения сгенерированы" << endl;
							}
						}
					}
				} while (action_number != "4");
			}
			else {
				if (In_Data2 == 0) {
					Input2(mixture);
					cout << "Введите количество генерируемых значений: n = ";
					cin >> size;
					while (size < 0)
					{
						cout << "Некоректный ввод данных.";
						cout << "Введите количество генерируемых значений: n = ";
						cin >> size;
					}
					In_Data2 = true;
				}
				f_2 = new float[size];
				X_2 = new float[size];
				Empiric empiricalFunction(size, mixture, 0);
				do {
					if (In_Data2 == 0) {
						Input2(mixture);
						cout << "Введите количество генерируемых значений: n = ";
						cin >> size;
						while (size < 0)
						{
							cout << "Некоректный ввод данных.";
							cout << "Введите количество генерируемых значений: n = ";
							cin >> size;
						}
						In_Data2 = true;
						delete[] f_2;
						delete[]X_2;
						f_2 = new float[size];
						X_2 = new float[size];
						Empiric empiricalFunction(size, mixture, 0);
					}

					do {
						cout << "Выберите действие из списка:" << endl;
						cout << "1. Вычисление плотности" << endl;
						cout << "2. Вычисление математического ожидания, дисперсии, коэффициентов асимметрии и эксцесса" << endl;
						cout << "3. Генерация значений функции плотности" << endl;
						cout << "4. Выход" << endl;
						cout << "Введите номер действия - ";
						cin >> action_number; cout << endl;
					} while (action_number != "1" and action_number != "2" and action_number != "3" and action_number != "4");

					if (action_number == "1") {
						cout << "Вычисление значений плотности распределения по аргументу" << endl;
						cout << "Введите значение x = "; cin >> x; cout << endl;
						f = empiricalFunction.get_density_by_dataset(x);
						cout << "Значение плотности распределения по аргументу = " << f << endl;
					}
					if (action_number == "2") {
						cout << "Вычисление математического ожидания, дисперсии, коэффициентов асимметрии и эксцесса" << endl;
						float* array = empiricalFunction.distribution_parameters_3();
						cout << "Математическое ожидание = " << array[0] << endl;
						cout << "Дисперсия = " << array[1] << endl;
						cout << "Коэффициент ассиметрии = " << array[2] << endl;
						cout << "Коэффициент Эксцесса = " << array[3] << endl;
					}
					if (action_number == "3") {
						cout << "Моделирование случайной величины" << endl;
						i = 0;
						regeneration = '1';
						do {
							if (regeneration != '1') cout << "Некоректный ввод данных.";
							cout << "Вы хотите перегенирировать выборку?" << endl;
							cout << "1. Да" << endl;
							cout << "2. Нет" << endl;
							cin >> regeneration;
						} while (regeneration != '1' and regeneration != '2');
						if (regeneration == '1') In_Data2 = false;
						if (In_Data2 == true) {
							do {
								cout << "Выберите способ генерации значений:" << endl;
								cout << "1. Ввод с клавиатуры" << endl;
								cout << "2. Автоматическая генерация" << endl;
								cout << "Введите способ генерации значений - "; cin >> regeneration; cout << endl;
							} while (regeneration != '1' and regeneration != '2');
							if (regeneration == '1') {
								out.open("output3.txt"); // открываем файл для записи
								for (int i = 0; i < size; i++) {

									cout << "x[" << i + 1 << "] = ";
									cin >> X_2[i];
									cout << endl;
									cout << "f[" << i + 1 << "] = ";
									cin >> f_2[i];
									cout << endl;
									if (out.is_open()) {
										out << X_2[i] << " " << f_2[i] << endl;
									}
								}
								out.close();
								empiricalFunction.setData(X_2);
								empiricalFunction.setFr(f_2);
							}
							else {
								out.open("output3.txt"); // открываем файл для записи
								X_2 = empiricalFunction.getData();
								f_2 = empiricalFunction.getFrequencies();
								i = 0;
								if (out.is_open()) {
									while (i < size) {
										out << X_2[i] << " " << f_2[i] << endl;
										i++;
									}
								}
								out.close();
								cout << "Значения сгенерированы" << endl;
							}
						}
					}
				} while (action_number != "4");
			}
		}
	} while (distribution_number != "4");

	err = fopen_s(&file, "output.txt", "w");
	if (err == 0) {
		cout << "Введите номер ";
		cin >> action_number;
		cout << endl;
		if (action_number == "1") {
			primary.save(file); // Сохраняем данные в файл
		}
		else if (action_number == "2")
		{
			mixture.load_mix(file);
		}
		else if (action_number == "3")
		{
			empiricalFunction.load_empiric(file);
		}
		cout << "Данные успешно загружены из output.txt." << endl;
		fclose(file); // Закрываем файл	
		std::cout << "Данные успешно сохранены в output.txt." << std::endl;
	}
	else {
		std::cerr << "Ошибка: не удалось открыть файл для записи." << std::endl;
	}
}