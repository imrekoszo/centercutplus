// implements class
#include "ffengine.h"

// standard headers
#include <math.h>

// locals
namespace
{
    const double kTwoPi = 6.283185307179586476925286766559;
    const double kInvSqrt2 = 0.70710678118654752440084436210485;
}

unsigned FFEngine::IntegerLog2(unsigned v)
{
    unsigned i = 0;

    while(v > 1)
    {
        ++i;
        v >>= 1;
    }

    return i;
}

unsigned FFEngine::RevBits(unsigned x, unsigned bits)
{
    unsigned y = 0;

    while(bits--)
    {
        y = (y + y) + (x & 1);
        x >>= 1;
    }

    return y;
}

void FFEngine::VDCreateRaisedCosineWindow(double *dst, int n, double power)
{
    const double twopi_over_n = kTwoPi/n;
    const double scalefac = 1.0/n;

    for(int i = 0; i < n; ++i)
    {
        dst[i] = scalefac * pow(0.5*(1.0 - cos(twopi_over_n * (i+0.5))), power);
    }
}

void FFEngine::VDCreateHalfSineTable(double *dst, int n)
{
    const double twopi_over_n = kTwoPi / n;

    for(int i = 0; i < n; ++i)
    {
        dst[i] = sin(twopi_over_n * i);
    }
}

void FFEngine::VDCreateBitRevTable(unsigned *dst, int n)
{
    unsigned bits = IntegerLog2(n);

    for(int i = 0; i < n; ++i)
    {
        dst[i] = RevBits(i, bits);
    }
}

void FFEngine::CreatePostWindow(double *dst, int windowSize, int power)
{
    const double powerIntegrals[8] =
                            { 1.0, 1.0/2.0, 3.0/8.0, 5.0/16.0, 35.0/128.0,
                              63.0/256.0, 231.0/1024.0, 429.0/2048.0 };
    const double scalefac = static_cast<double>(windowSize) *
                            (powerIntegrals[1] / powerIntegrals[power+1]);

    VDCreateRaisedCosineWindow(dst, windowSize, static_cast<double>(power));

    for(int i = 0; i < windowSize; ++i)
    {
        dst[i] *= scalefac;
    }
}

void FFEngine::VDComputeFHT(double *A, int nPoints, const double *sinTab)
{
    // FHT - stage 1 and 2 (2 and 4 points)
    for(int i = 0; i < nPoints; i += 4)
    {
        const double x0 = A[i];
        const double x1 = A[i + 1];
        const double x2 = A[i + 2];
        const double x3 = A[i + 3];

        const double y0 = x0 + x1;
        const double y1 = x0 - x1;
        const double y2 = x2 + x3;
        const double y3 = x2 - x3;

        A[i]     = y0 + y2;
        A[i + 2] = y0 - y2;

        A[i + 1] = y1 + y3;
        A[i + 3] = y1 - y3;
    }

    // FHT - stage 3 (8 points)
    for(int i = 0; i < nPoints; i += 8)
    {
        double alpha, beta;

        alpha    = A[i + 0];
        beta     = A[i + 4];

        A[i + 0] = alpha + beta;
        A[i + 4] = alpha - beta;

        alpha    = A[i + 2];
        beta     = A[i + 6];

        A[i + 2] = alpha + beta;
        A[i + 6] = alpha - beta;

        alpha    = A[i + 1];

        const double beta1 = kInvSqrt2*(A[i + 5] + A[i + 7]);
        const double beta2 = kInvSqrt2*(A[i + 5] - A[i + 7]);

        A[i + 1] = alpha + beta1;
        A[i + 5] = alpha - beta1;

        alpha    = A[i + 3];

        A[i + 3] = alpha + beta2;
        A[i + 7] = alpha - beta2;
    }

    int n = 16;
    int n2 = 8;
    int theta_inc = nPoints >> 4;

    while(n <= nPoints)
    {
        for(int i = 0; i < nPoints; i += n)
        {
            int theta = theta_inc;
            const int n4 = n2>>1;
            double alpha, beta;

            alpha	= A[i];
            beta	= A[i + n2];

            A[i]	= alpha + beta;
            A[i + n2]	= alpha - beta;

            alpha	= A[i + n4];
            beta	= A[i + n2 + n4];

            A[i + n4]		= alpha + beta;
            A[i + n2 + n4]	= alpha - beta;

            for(int j = 1; j < n4; ++j)
            {
                double sinval = sinTab[theta];
                double cosval = sinTab[theta + (nPoints >> 2)];

                double alpha1 = A[i + j];
                double alpha2 = A[i - j + n2];
                double beta1  = A[i + j + n2]*cosval + A[i - j + n]*sinval;
                double beta2  = A[i + j + n2]*sinval - A[i - j + n]*cosval;

                theta += theta_inc;

                A[i + j]      = alpha1 + beta1;
                A[i + j + n2] = alpha1 - beta1;
                A[i - j + n2] = alpha2 + beta2;
                A[i - j + n]  = alpha2 - beta2;
            }
        }

        n *= 2;
        n2 *= 2;
        theta_inc >>= 1;
    }
}
