#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "calculatorcore_global.h"

class CALCULATORCORESHARED_EXPORT Calculator {
public:
    Calculator();
    int Add(int a, int b);
    double Multiply(double a, double b);
    void Manipulate(double* data, unsigned length, double multiplier);
};

#endif // CALCULATOR_H
