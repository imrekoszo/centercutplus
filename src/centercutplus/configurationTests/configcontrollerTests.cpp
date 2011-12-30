#include <boost/scoped_ptr.hpp>

#include <centercutplus/core/config.h>
#include <centercutplus/configuration/configcontroller.h>
#include <centercutplus/configuration/iconfigviewmodel.h>

#define NOMINMAX
#include <WinUnit.h>
#include <tests/random.h>

using ccp::configuration::ConfigController;

namespace
{

boost::scoped_ptr<ccp::core::Config> config;
boost::scoped_ptr<ConfigController> sut;
const ccp::configuration::IConfigViewModel* model;

void Setup()
{
    sut.reset();
    config.reset(new ccp::core::Config());
    sut.reset(new ConfigController(*config));
    model = sut->Model();
}

}


BEGIN_TEST(Controller_SetBypassedTest)
{
    // arrange
    Setup();
    config->Bypass(false);
    auto value = true;

    // act
    sut->SetBypassed(value);

    // assert
    WIN_ASSERT_EQUAL(value, model->GetBypassed());
}
END_TEST

BEGIN_TEST(Controller_SetCenterDetectionModeTest)
{
    // arrange
    Setup();
    config->engineConfig().centerDetectionMode(ccp::core::kCenterDetectionMode_BothPhaseLeft);
    auto value = ccp::core::kCenterDetectionMode_InversePhaseRight;

    // act
    sut->SetCenterDetectionMode(value);

    // assert
    WIN_ASSERT_EQUAL(value, model->GetCurrentEngineConfig().centerDetectionMode());
}
END_TEST

BEGIN_TEST(Controller_SetLeftToLeftPercentTest)
{
    // arrange
    Setup();
    config->engineConfig().leftToLeftPercent(Random::Percent());
    auto value = Random::Percent();

    // act
    sut->SetLeftToLeftPercent(value);

    // assert
    WIN_ASSERT_EQUAL(value, model->GetCurrentEngineConfig().leftToLeftPercent());
}
END_TEST

BEGIN_TEST(Controller_SetCenterToLeftPercentTest)
{
    // arrange
    Setup();
    config->engineConfig().centerToLeftPercent(Random::Percent());
    auto value = Random::Percent();

    // act
    sut->SetCenterToLeftPercent(value);

    // assert
    WIN_ASSERT_EQUAL(value, model->GetCurrentEngineConfig().centerToLeftPercent());
}
END_TEST

BEGIN_TEST(Controller_SetRightToLeftPercentTest)
{
    // arrange
    Setup();
    config->engineConfig().rightToLeftPercent(Random::Percent());
    auto value = Random::Percent();

    // act
    sut->SetRightToLeftPercent(value);

    // assert
    WIN_ASSERT_EQUAL(value, model->GetCurrentEngineConfig().rightToLeftPercent());
}
END_TEST

BEGIN_TEST(Controller_SetLeftToRightPercentTest)
{
    // arrange
    Setup();
    config->engineConfig().leftToRightPercent(Random::Percent());
    auto value = Random::Percent();

    // act
    sut->SetLeftToRightPercent(value);

    // assert
    WIN_ASSERT_EQUAL(value, model->GetCurrentEngineConfig().leftToRightPercent());
}
END_TEST

BEGIN_TEST(Controller_SetCenterToRightPercentTest)
{
    // arrange
    Setup();
    config->engineConfig().centerToRightPercent(Random::Percent());
    auto value = Random::Percent();

    // act
    sut->SetCenterToRightPercent(value);

    // assert
    WIN_ASSERT_EQUAL(value, model->GetCurrentEngineConfig().centerToRightPercent());
}
END_TEST

BEGIN_TEST(Controller_SetRightToRightPercentTest)
{
    // arrange
    Setup();
    config->engineConfig().rightToRightPercent(Random::Percent());
    auto value = Random::Percent();

    // act
    sut->SetRightToRightPercent(value);

    // assert
    WIN_ASSERT_EQUAL(value, model->GetCurrentEngineConfig().rightToRightPercent());
}
END_TEST
