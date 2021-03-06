/**
    @file   Equation.h
    @brief  N次方程式クラス定義
*/

#ifndef INCLUDE_APIDES_EQUATION_EQUATION_H
#define INCLUDE_APIDES_EQUATION_EQUATION_H

#include <dll-export.h>

#include <memory>
#include <iterator>

namespace equation {

/**
    @brief  N次方程式クラス
*/
DLL_PUBLIC
class Equation {
    struct Impl;
    std::unique_ptr<Equation::Impl> impl;

    Equation() = delete;
    Equation(Equation const&) = delete;
    Equation(Equation &&) = delete;
    Equation & operator=(Equation const&) = delete;
    Equation & operator=(Equation &&) = delete;

public:
    Equation(const double coeffs[], size_t size);
    Equation(std::const_iterator<double> coeff_st, std::const_iterator<double> coeff_ed);
    ~Equation();

    double calc(double x) const;

    void setBegin(double x);
    void setStep(double step);
    double calcNext();
};

} // namespace equation

#endif //INCLUDE_APIDES_EQUATION_EQUATION_H
