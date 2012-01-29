#include <centercutplus/common/types.h>
#include <centercutplus/core/engineconfig.h>

#define NOMINMAX
#include <WinUnit.h>

using ccp::uint;
using ccp::core::EngineConfig;

namespace
{

EngineConfig sut;

void ResetSut()
{
    sut
        .centerDetectionMode(ccp::core::kCenterDetectionMode_InPhase)
        .centerToLeftPercent(0)
        .centerToRightPercent(0)
        .leftToLeftPercent(0)
        .leftToRightPercent(0)
        .rightToLeftPercent(0)
        .rightToRightPercent(0)
        .focusPositionPercent(0)
        .centerToSidesFrequencyIntervals().clear();
}

}

BEGIN_TEST(Should_sanitize_max_centerToLeftPercent)
{
    // arrange
    ResetSut();
    int outOfRange = EngineConfig::kMaxPercent + 1;

    // act
    sut.centerToLeftPercent(outOfRange);

    // assert
    WIN_ASSERT_EQUAL(sut.centerToLeftPercent(), EngineConfig::kMaxPercent);
}
END_TEST

BEGIN_TEST(Should_sanitize_max_centerToRightPercent)
{
    // arrange
    ResetSut();
    int outOfRange = EngineConfig::kMaxPercent + 1;

    // act
    sut.centerToRightPercent(outOfRange);

    // assert
    WIN_ASSERT_EQUAL(sut.centerToRightPercent(), EngineConfig::kMaxPercent);
}
END_TEST

BEGIN_TEST(Should_sanitize_max_leftToLeftPercent)
{
    // arrange
    ResetSut();
    int outOfRange = EngineConfig::kMaxPercent + 1;

    // act
    sut.leftToLeftPercent(outOfRange);

    // assert
    WIN_ASSERT_EQUAL(sut.leftToLeftPercent(), EngineConfig::kMaxPercent);
}
END_TEST

BEGIN_TEST(Should_sanitize_max_leftToRightPercent)
{
    // arrange
    ResetSut();
    int outOfRange = EngineConfig::kMaxPercent + 1;

    // act
    sut.leftToRightPercent(outOfRange);

    // assert
    WIN_ASSERT_EQUAL(sut.leftToRightPercent(), EngineConfig::kMaxPercent);
}
END_TEST

BEGIN_TEST(Should_sanitize_max_rightToLeftPercent)
{
    // arrange
    ResetSut();
    int outOfRange = EngineConfig::kMaxPercent + 1;

    // act
    sut.rightToLeftPercent(outOfRange);

    // assert
    WIN_ASSERT_EQUAL(sut.rightToLeftPercent(), EngineConfig::kMaxPercent);
}
END_TEST

BEGIN_TEST(Should_sanitize_max_rightToRightPercent)
{
    // arrange
    ResetSut();
    int outOfRange = EngineConfig::kMaxPercent + 1;

    // act
    sut.rightToRightPercent(outOfRange);

    // assert
    WIN_ASSERT_EQUAL(sut.rightToRightPercent(), EngineConfig::kMaxPercent);
}
END_TEST

BEGIN_TEST(Should_sanitize_max_focusPositionPercent)
{
    // arrange
    ResetSut();
    int outOfRange = EngineConfig::kMaxPercent + 1;

    // act
    sut.focusPositionPercent(outOfRange);

    // assert
    WIN_ASSERT_EQUAL(sut.focusPositionPercent(), EngineConfig::kMaxPercent);
}
END_TEST

BEGIN_TEST(Should_sanitize_min_centerToLeftPercent)
{
    // arrange
    ResetSut();
    int outOfRange = EngineConfig::kMinPercent - 1;

    // act
    sut.centerToLeftPercent(outOfRange);

    // assert
    WIN_ASSERT_EQUAL(sut.centerToLeftPercent(), EngineConfig::kMinPercent);
}
END_TEST

BEGIN_TEST(Should_sanitize_min_centerToRightPercent)
{
    // arrange
    ResetSut();
    int outOfRange = EngineConfig::kMinPercent - 1;

    // act
    sut.centerToRightPercent(outOfRange);

    // assert
    WIN_ASSERT_EQUAL(sut.centerToRightPercent(), EngineConfig::kMinPercent);
}
END_TEST

BEGIN_TEST(Should_sanitize_min_leftToLeftPercent)
{
    // arrange
    ResetSut();
    int outOfRange = EngineConfig::kMinPercent - 1;

    // act
    sut.leftToLeftPercent(outOfRange);

    // assert
    WIN_ASSERT_EQUAL(sut.leftToLeftPercent(), EngineConfig::kMinPercent);
}
END_TEST

BEGIN_TEST(Should_sanitize_min_leftToRightPercent)
{
    // arrange
    ResetSut();
    int outOfRange = EngineConfig::kMinPercent - 1;

    // act
    sut.leftToRightPercent(outOfRange);

    // assert
    WIN_ASSERT_EQUAL(sut.leftToRightPercent(), EngineConfig::kMinPercent);
}
END_TEST

BEGIN_TEST(Should_sanitize_min_rightToLeftPercent)
{
    // arrange
    ResetSut();
    int outOfRange = EngineConfig::kMinPercent - 1;

    // act
    sut.rightToLeftPercent(outOfRange);

    // assert
    WIN_ASSERT_EQUAL(sut.rightToLeftPercent(), EngineConfig::kMinPercent);
}
END_TEST

BEGIN_TEST(Should_sanitize_min_rightToRightPercent)
{
    // arrange
    ResetSut();
    int outOfRange = EngineConfig::kMinPercent - 1;

    // act
    sut.rightToRightPercent(outOfRange);

    // assert
    WIN_ASSERT_EQUAL(sut.rightToRightPercent(), EngineConfig::kMinPercent);
}
END_TEST

BEGIN_TEST(Should_sanitize_min_focusPositionPercent)
{
    // arrange
    ResetSut();
    int outOfRange = EngineConfig::kMinPercent - 1;

    // act
    sut.focusPositionPercent(outOfRange);

    // assert
    WIN_ASSERT_EQUAL(sut.focusPositionPercent(), EngineConfig::kMinPercent);
}
END_TEST

BEGIN_TEST(Should_sanitize_max_centerDetectionMode)
{
    // arrange
    ResetSut();

    // act
    sut.centerDetectionMode((ccp::core::CenterDetectionMode)(ccp::core::kCenterDetectionMode_Max + 1));

    // assert
    WIN_ASSERT_EQUAL(sut.centerDetectionMode(), ccp::core::kCenterDetectionMode_InPhase);
}
END_TEST

BEGIN_TEST(Should_sanitize_min_centerDetectionMode)
{
    // arrange
    ResetSut();

    // act
    sut.centerDetectionMode((ccp::core::CenterDetectionMode)(ccp::core::kCenterDetectionMode_Min - 1));

    // assert
    WIN_ASSERT_EQUAL(sut.centerDetectionMode(), ccp::core::kCenterDetectionMode_InPhase);
}
END_TEST
