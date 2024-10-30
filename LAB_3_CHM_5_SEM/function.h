#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>
#include "coordinate.h"

class SmoothSpline {
private:
    double smoothing_factor;
    std::vector<Coordinate> nodes;
    std::vector<double> coefficients;
public:
    SmoothSpline(const double& smoothing_factor);
    void mapToMasterElement(int segment_num, const double& x_coord, double& local_coord) const;
    double baseFunc(int func_num, const double& local_coord) const;
    double baseFuncDerivative(int func_num, const double& local_coord) const;
    void fitSpline(const std::vector<Coordinate>& coordinates, const std::vector<double>& func_values);
    void evaluateSpline(const Coordinate& point, double* result) const;
};

SmoothSpline::SmoothSpline(const double& smoothing_factor) {
    this->smoothing_factor = smoothing_factor;
}

double SmoothSpline::baseFuncDerivative(int func_num, const double& local_coord) const {
    if (func_num == 1) {
        return -0.5;
    }
    else if (func_num == 2) {
        return 0.5;
    }
    else {
        throw std::invalid_argument("Invalid derivative index");
    }
}

void SmoothSpline::mapToMasterElement(int segment_num, const double& x_coord, double& local_coord) const {
    local_coord = 2.0 * (x_coord - nodes[segment_num].getX()) / (nodes[segment_num + 1].getX() - nodes[segment_num].getX()) - 1.0;
}

double SmoothSpline::baseFunc(int func_num, const double& local_coord) const {
    if (func_num == 1) {
        return 0.5 * (1 - local_coord);
    }
    else if (func_num == 2) {
        return 0.5 * (1 + local_coord);
    }
    else {
        throw std::invalid_argument("Invalid function index");
    }
}

void SmoothSpline::fitSpline(const std::vector<Coordinate>& coordinates, const std::vector<double>& func_values) {
    nodes.clear();
    for (const auto& coord : coordinates) {
        nodes.push_back(coord);
    }

    int num_segments = coordinates.size() - 1;
    coefficients.resize(num_segments + 1);

    std::vector<double> lower_diagonal(num_segments + 1), main_diagonal(num_segments + 1), upper_diagonal(num_segments + 1);

    auto assemble = [&](int segment_index, const Coordinate& coord, const double& func_value, const double& weight) {
        double x = coord.getX(), ksi;
        mapToMasterElement(segment_index, x, ksi);
        double f1 = baseFunc(1, ksi);
        double f2 = baseFunc(2, ksi);

        main_diagonal[segment_index] += (1.0 - smoothing_factor) * weight * f1 * f1;
        main_diagonal[segment_index + 1] += (1.0 - smoothing_factor) * weight * f2 * f2;
        lower_diagonal[segment_index + 1] += (1.0 - smoothing_factor) * weight * f1 * f2;
        upper_diagonal[segment_index] += (1.0 - smoothing_factor) * weight * f2 * f1;

        coefficients[segment_index] += (1.0 - smoothing_factor) * weight * f1 * func_value;
        coefficients[segment_index + 1] += (1.0 - smoothing_factor) * weight * f2 * func_value;
        };

    for (int i = 0; i < num_segments; i++) {
        double weight = 1.0;
        assemble(i, nodes[i], func_values[i], weight);
        assemble(i, nodes[i + 1], func_values[i + 1], weight);

        double segment_length = coordinates[i + 1].getX() - coordinates[i].getX();
        main_diagonal[i] += 1.0 / segment_length * smoothing_factor;
        main_diagonal[i + 1] += 1.0 / segment_length * smoothing_factor;
        lower_diagonal[i + 1] -= 1.0 / segment_length * smoothing_factor;
        upper_diagonal[i] -= 1.0 / segment_length * smoothing_factor;
    }

    for (int j = 1; j < num_segments + 1; j++) {
        main_diagonal[j] -= lower_diagonal[j] / main_diagonal[j - 1] * upper_diagonal[j - 1];
        coefficients[j] -= lower_diagonal[j] / main_diagonal[j - 1] * coefficients[j - 1];
    }

    coefficients[num_segments] /= main_diagonal[num_segments];
    for (int j = num_segments - 1; j >= 0; j--) {
        coefficients[j] = (coefficients[j] - coefficients[j + 1] * upper_diagonal[j]) / main_diagonal[j];
    }
}

void SmoothSpline::evaluateSpline(const Coordinate& point, double* result) const {
    double tolerance = 1e-7;
    int num_segments = nodes.size() - 1;
    double x = point.getX();

    for (int i = 0; i < num_segments; i++) {
        if ((x > nodes[i].getX() && x < nodes[i + 1].getX()) ||
            std::fabs(x - nodes[i].getX()) < tolerance || std::fabs(x - nodes[i + 1].getX()) < tolerance) {
            double segment_length = nodes[i + 1].getX() - nodes[i].getX();
            double local_coord;
            mapToMasterElement(i, x, local_coord);

            *result = coefficients[i] * baseFunc(1, local_coord) + coefficients[i + 1] * baseFunc(2, local_coord);
            return;
        }
    }
    throw std::runtime_error("Point not within segments");
}
