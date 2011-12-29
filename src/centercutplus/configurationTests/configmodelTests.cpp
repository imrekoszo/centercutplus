#include <boost/scoped_ptr.hpp>

#include <centercutplus/core/config.h>
#include "../configuration/src/configmodel.h"

#define NOMINMAX
#include <WinUnit.h>
#include <tests/random.h>

#include "view.h"

using ccp::configuration::ConfigModel;

namespace
{

boost::scoped_ptr<ccp::core::Config> config;
boost::scoped_ptr<ConfigModel> sut;
boost::scoped_ptr<View> view;

void Setup()
{
    sut.reset();
    config.reset(new ccp::core::Config());
    view.reset(new View());
    sut.reset(new ConfigModel(*config));
    sut->Subscribe(*view);
}

}

BEGIN_TEST(GetBypassedTest)
{
    // arrange
    Setup();

    // assert
    WIN_ASSERT_EQUAL(config->IsBypassed(), sut->GetBypassed());
}
END_TEST

BEGIN_TEST(GetCurrentEngineConfigTest)
{
    // arrange
    Setup();

    // assert
    WIN_ASSERT_EQUAL(&(config->engineConfig()), &(sut->GetCurrentEngineConfig()));
}
END_TEST

BEGIN_TEST(UnsubscribeTest)
{
    // arrange
    Setup();

    // act
    sut->Unsubscribe(*view);

    sut->SetBypassed(true, view.get());
    sut->SetCenterDetectionMode(ccp::core::kCenterDetectionMode_BothPhaseLeft, view.get());
    sut->SetLeftToLeftPercent(Random::Percent(), view.get());
    sut->SetCenterToLeftPercent(Random::Percent(), view.get());
    sut->SetRightToLeftPercent(Random::Percent(), view.get());
    sut->SetLeftToRightPercent(Random::Percent(), view.get());
    sut->SetCenterToRightPercent(Random::Percent(), view.get());
    sut->SetRightToRightPercent(Random::Percent(), view.get());

    // assert
    WIN_ASSERT_FALSE(view->Updated);
    WIN_ASSERT_NULL(view->Origin);
}
END_TEST

BEGIN_TEST(SetBypassedTest)
{
    // arrange
    Setup();
    config->Bypass(false);

    // act
    sut->SetBypassed(true, view.get());

    // assert
    WIN_ASSERT_EQUAL(true, config->IsBypassed());
    WIN_ASSERT_TRUE(view->Updated);
    WIN_ASSERT_EQUAL(view.get(), view->Origin);
}
END_TEST

BEGIN_TEST(SetCenterDetectionModeTest)
{
    // arrange
    Setup();
    config->engineConfig().centerDetectionMode(ccp::core::kCenterDetectionMode_BothPhaseLeft);
    auto value = ccp::core::kCenterDetectionMode_BothPhaseRight;

    // act
    sut->SetCenterDetectionMode(value, view.get());

    // assert
    WIN_ASSERT_EQUAL(value, config->engineConfig().centerDetectionMode());
    WIN_ASSERT_TRUE(view->Updated);
    WIN_ASSERT_EQUAL(view.get(), view->Origin);
}
END_TEST

BEGIN_TEST(SetLeftToLeftPercentTest)
{
    // arrange
    Setup();
    config->engineConfig().leftToLeftPercent(Random::Percent());
    auto value = Random::Percent();

    // act
    sut->SetLeftToLeftPercent(value, view.get());

    // assert
    WIN_ASSERT_EQUAL(value, config->engineConfig().leftToLeftPercent());
    WIN_ASSERT_TRUE(view->Updated);
    WIN_ASSERT_EQUAL(view.get(), view->Origin);
}
END_TEST

BEGIN_TEST(SetLeftToRightPercentTest)
{
    // arrange
    Setup();
    config->engineConfig().leftToRightPercent(Random::Percent());
    auto value = Random::Percent();

    // act
    sut->SetLeftToRightPercent(value, view.get());

    // assert
    WIN_ASSERT_EQUAL(value, config->engineConfig().leftToRightPercent());
    WIN_ASSERT_TRUE(view->Updated);
    WIN_ASSERT_EQUAL(view.get(), view->Origin);
}
END_TEST

BEGIN_TEST(SetRightToLeftPercentTest)
{
    // arrange
    Setup();
    config->engineConfig().rightToLeftPercent(Random::Percent());
    auto value = Random::Percent();

    // act
    sut->SetRightToLeftPercent(value, view.get());

    // assert
    WIN_ASSERT_EQUAL(value, config->engineConfig().rightToLeftPercent());
    WIN_ASSERT_TRUE(view->Updated);
    WIN_ASSERT_EQUAL(view.get(), view->Origin);
}
END_TEST

BEGIN_TEST(SetRightToRightPercentTest)
{
    // arrange
    Setup();
    config->engineConfig().rightToRightPercent(Random::Percent());
    auto value = Random::Percent();

    // act
    sut->SetRightToRightPercent(value, view.get());

    // assert
    WIN_ASSERT_EQUAL(value, config->engineConfig().rightToRightPercent());
    WIN_ASSERT_TRUE(view->Updated);
    WIN_ASSERT_EQUAL(view.get(), view->Origin);
}
END_TEST

BEGIN_TEST(SetCenterToLeftPercentTest)
{
    // arrange
    Setup();
    config->engineConfig().centerToLeftPercent(Random::Percent());
    auto value = Random::Percent();

    // act
    sut->SetCenterToLeftPercent(value, view.get());

    // assert
    WIN_ASSERT_EQUAL(value, config->engineConfig().centerToLeftPercent());
    WIN_ASSERT_TRUE(view->Updated);
    WIN_ASSERT_EQUAL(view.get(), view->Origin);
}
END_TEST

BEGIN_TEST(SetCenterToRightPercentTest)
{
    // arrange
    Setup();
    config->engineConfig().centerToRightPercent(Random::Percent());
    auto value = Random::Percent();

    // act
    sut->SetCenterToRightPercent(value, view.get());

    // assert
    WIN_ASSERT_EQUAL(value, config->engineConfig().centerToRightPercent());
    WIN_ASSERT_TRUE(view->Updated);
    WIN_ASSERT_EQUAL(view.get(), view->Origin);
}
END_TEST
