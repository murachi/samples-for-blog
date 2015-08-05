/**
    @file   Equation.cpp
    @brief  N次方程式クラス実装
*/

#include "Equation.h"

#include <vector>
#include <cmath>

struct Equation::Impl {
    std::vector<double> coeffs;
    double x_pos;
};

Equation::Equation(const double coeffs[], size_t size) : impl{new Equation::Impl{}}
{
    if (size <= 0) return;
    impl->coeffs.assign(coeffs, coeffs + size - 1);
}

Equation::Equation(std::const_iterator<double> coeff_st, std::const_iterator<double> coeff_ed) :
    impl{new Equation::Impl{}}
{
    impl->coeffs.assign(coeff_st, coeff_ed);
}

Equation::~Equation() = default;

double Equation::calc(double x) const
{
    double y = 0;
    for (size_t i = 0; i < impl->coeffs.size(); i++) y += impl->coeffs[i] * pow(x, i);
    return y;
}

void Equation::setBegin(double x)
{
}

void Equation::setStep(double step)
{
}

double Equation::calcNext()
{
}

