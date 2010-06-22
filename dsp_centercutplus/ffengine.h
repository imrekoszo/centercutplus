#ifndef FFENGINE_H
#define FFENGINE_H

class FFEngine
{
    FFEngine() { }

    static const double TwoPi;
    static const double InvSqrt2;

public:
    static unsigned IntegerLog2(unsigned v);
    static unsigned RevBits(unsigned x, unsigned bits);
    static void VDCreateRaisedCosineWindow(double *dst, int n, double power);
    static void VDCreateHalfSineTable(double *dst, int n);
    static void VDCreateBitRevTable(unsigned *dst, int n);
    static void CreatePostWindow(double *dst, int windowSize, int power);
    static void VDComputeFHT(double *A, int nPoints, const double *sinTab);
};

#endif // FFENGINE_H
