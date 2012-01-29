#include <centercutplus/common/types.h>
#include <centercutplus/core/frequencyinterval.h>

#define NOMINMAX
#include <WinUnit.h>
#include <tests/random.h>

using ccp::uint;
using ccp::core::FrequencyInterval;

namespace
{

FrequencyInterval sut;

void ResetSut()
{
    sut.minimum(FrequencyInterval::kMinFrequency);
    sut.maximum(FrequencyInterval::kMaxFrequency);
}

}

BEGIN_TEST(Should_start_with_empty_interval)
{
    // arrange
    FrequencyInterval localSut;

    // act

    // assert
    WIN_ASSERT_EQUAL(localSut.minimum(), localSut.maximum());
}
END_TEST

BEGIN_TEST(Should_sanitize_max_against_max_frequency)
{
    // arrange
    ResetSut();

    // act
    sut.maximum(FrequencyInterval::kMaxFrequency + 1u);

    // assert
    WIN_ASSERT_EQUAL(FrequencyInterval::kMaxFrequency, sut.maximum());
}
END_TEST

BEGIN_TEST(Should_sanitize_min_against_min_frequency)
{
    // arrange
    ResetSut();

    // act
    sut.minimum(FrequencyInterval::kMinFrequency - 1u);

    // assert
    WIN_ASSERT_EQUAL(FrequencyInterval::kMinFrequency, sut.minimum());
}
END_TEST

BEGIN_TEST(Should_sanitize_max_against_min)
{
    // arrange
    ResetSut();
    uint min = Random::Frequency();
    sut.minimum(min);

    // act
    sut.maximum(min - 1u);

    // assert
    WIN_ASSERT_EQUAL(min, sut.maximum());
}
END_TEST

BEGIN_TEST(Should_sanitize_min_against_max)
{
    // arrange
    ResetSut();
    uint max = Random::Frequency();
    sut.maximum(max);

    // act
    sut.minimum(max + 1u);

    // assert
    WIN_ASSERT_EQUAL(max, sut.minimum());
}
END_TEST

BEGIN_TEST(IsInInterval_should_give_correct_result)
{
    // arrange
    ResetSut();
    uint max = Random::Uint(
        FrequencyInterval::kMinFrequency + 1u +
            (FrequencyInterval::kMaxFrequency - FrequencyInterval::kMinFrequency + 1u) / 2u,
        FrequencyInterval::kMaxFrequency);
    uint min = Random::Uint(FrequencyInterval::kMinFrequency + 1u, max);
    uint tooBig = max + 1u;
    uint tooSmall = min - 1u;
    uint good = min + (max - min) / 2u;

    sut.minimum(min);
    sut.maximum(max);

    // assert
    WIN_ASSERT_TRUE(sut.IsInInterval(max));
    WIN_ASSERT_TRUE(sut.IsInInterval(min));
    WIN_ASSERT_TRUE(sut.IsInInterval(good));
    WIN_ASSERT_FALSE(sut.IsInInterval(tooBig));
    WIN_ASSERT_FALSE(sut.IsInInterval(tooSmall));
}
END_TEST
