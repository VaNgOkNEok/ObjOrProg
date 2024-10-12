#include <iostream>
#include <stdexcept> // ��� ����������
#include "Primary.h"

class Mixture {
private:
    float p;         // ����������� ��� �����
    Primary d1, d2;  // ��� ���������� �����

public:
    // ����������� � �������������� � ��������� ����������� p
    Mixture(Primary& prim1, Primary& prim2, float p0) :
        p(p0 > 0 && p0 < 1 ? p0 : throw std::invalid_argument("p ������ ���� � ��������� (0,1)")), d1(prim1), d2(prim2) {}

    // ���������� ������ ����������
    Primary& component1();

    // ���������� ������ ����������
    Primary& component2();

    // ������ ��� ���������� 1
    void setComponent1(Primary& prim1) { d1 = prim1; }

    // ������ ��� ���������� 2
    void setComponent2(Primary& prim2) { d2 = prim2; }

    // ������ ��� ����������� p
    void setP(float p0) {
        if (p0 > 0 && p0 < 1) {
            p = p0;
        }
        else {
            throw std::invalid_argument("p ������ ���� � ��������� (0,1)");
        }
    }

    // ���������� �������� p
    float getP();

    // ��������� ��������� ������������� �����
    float distribution_density_2(const float x);

    // ���������� ��������� ������������� �����
    float* distribution_parameters_2();

    float* random_x_mixture(const int size);
    float* random_f_mixture(float* x, const int size);

    // ���������� ������ ��������� ������� ��� �����
    void random_number_mixture(int size);

    // ��������� ��������� ����� � ����
    void save_mix(FILE* stream) const;

    // ��������� ��������� ����� �� �����
    void load_mix(FILE* stream);
};