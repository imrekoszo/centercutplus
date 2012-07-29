#include "../inc/centercutplus/core/types.h"

namespace ccp
{
namespace core
{

class FhtEngine : boost::noncopyable
{
    // ctor
  private:
    FhtEngine();

    // methods
  public:
    static void VDCreateRaisedCosineWindow(double *dst, int n, double power);
    static void VDCreateHalfSineTable(double *dst, int n);
    static void VDCreateBitRevTable(unsigned *dst, int n);
    static void VDComputeFHT(double *A, int nPoints, const double *sinTab);
    static void VDCreatePostWindow(double *dst, int windowSize, int power);
  private:
    static uint IntegerLog2(uint v);
    static uint RevBits(uint x, uint bits);
};

}
}
