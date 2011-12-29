#include "../inc/tests/random.h"

#include <cstdlib>
#include <time.h>


#include <centercutplus/core/frequencyinterval.h>
#include <centercutplus/core/engineconfig.h>

namespace
{
bool InitSrand()
{
    srand(static_cast<unsigned>(time(NULL)));
    return true;
}
}

ccp::uint Random::Uint(ccp::uint min, ccp::uint max)
{
    Init();
    return min == max
        ? min
        : min + static_cast<ccp::uint>(rand() % static_cast<int>(max - min));
}

int Random::Int(int min, int max)
{
    Init();
    return min == max
        ? min
        : min + rand() % (max - min);
}

int Random::Percent()
{
    Init();
    return Int(ccp::core::EngineConfig::kMinPercent, ccp::core::EngineConfig::kMaxPercent);
}

ccp::uint Random::Frequency()
{
    Init();
    return Uint(ccp::core::FrequencyInterval::kMinFrequency, ccp::core::FrequencyInterval::kMaxFrequency);
}

void Random::Init()
{
    static bool initSrand = InitSrand();
}
