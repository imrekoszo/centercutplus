#include "calculator.h"

#include <fstream>
#include <sstream>

Calculator::Calculator()
{
}

int Calculator::Add(int a, int b)
{
    std::stringstream buf;
    buf << std::fixed;
    buf << "Add" << std::endl << a << std::endl << b << std::endl;

    int result = a + b;

    buf << result << std::endl;
    std::ofstream ofile("add.log", std::ios::app);
    ofile << buf.str();

    return result;
}

double Calculator::Multiply(double a, double b)
{
    std::stringstream buf;
    buf << std::fixed;
    buf << "Multiply" << std::endl << a << std::endl << b << std::endl;

    double result = a * b;

    buf << result << std::endl;
    std::ofstream ofile("multiply.log", std::ios::app);
    ofile << buf.str();

    return result;
}

void Calculator::Manipulate(double* data, unsigned length, double multiplier)
{
    std::stringstream buf;
    buf << std::fixed;
    buf << "Manipulate" << std::endl
            << length << std::endl
            << multiplier << std::endl;

    for(unsigned i = 0; i < length; ++i)
    {
        buf << data[i] << std::endl;
    }

    for(unsigned i = 0; i < length; ++i)
    {
        data[i] = Multiply(data[i], multiplier);
    }

    for(unsigned i = 0; i < length; ++i)
    {
        buf << data[i] << std::endl;
    }

    std::ofstream ofile("manipulate.log", std::ios::app);
    ofile << buf.str();
}
