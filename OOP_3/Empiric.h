#include <iostream>
#include "Primary.h"
#include "Mixture.h"

class Empiric {
private:
    int n;           
    int k;           
    float* data;     
    float* fr;       
    float max, min;  

public:
    
    Empiric(int n0, Primary& prim, int k0);
    Empiric(int n0, Mixture& mixt, int k0);
    Empiric(int n0, Empiric& emp, int k0);
    Empiric(const Empiric& emp);
    Empiric& operator=(const Empiric& emp);
    ~Empiric();

    int getN() const { return n; }
    float getMin() const { return min; }
    float getMax() const { return max; }
    float* getData() const { return data; }
    float* getFrequencies() const { return fr; }

    void setN(int n0);
    void setK(int k0);
    void setData(float* data);
    void setFr(float* fr);

    float get_density_by_dataset(const float x);
    void get_array_frequencies_Ni_elements(float*& ni_begin, float*& ni_end);
    float* obtain_values_density_empirical_function();

    void save_empiric(FILE* stream) const;
    void load_empiric(FILE* stream);

    float get_expectation_by_dataset(const float* dataset_begin, const float* dataset_end);
    float get_dispersion_by_dataset(const float* dataset_begin, const float* dataset_end);
    float get_excess_by_dataset(const float* dataset_begin, const float* dataset_end);
    float get_asymmetry_by_dataset(const float* dataset_begin, const float* dataset_end);
    float* distribution_parameters_3();
};