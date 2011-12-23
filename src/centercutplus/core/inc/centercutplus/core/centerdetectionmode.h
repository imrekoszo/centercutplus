#pragma once

namespace ccp
{
namespace core
{

///
/// Given the following signals as a given moment in the left and right channels:
/// L: l a b
/// R: r a -b
///
/// The modes should result in the following left right and center channels:
///                                              L             R            C
/// kCenterDetectionMode_InPhase                 l b           r -b         a
/// kCenterDetectionMode_InversePhaseLeft        l a           r  a         b
/// kCenterDetectionMode_InversePhaseRight       l a           r  a         -b
/// kCenterDetectionMode_BothPhaseLeft           l             r            a b
/// kCenterDetectionMode_BothPhaseRight          l             r            a -b
///
enum CenterDetectionMode
{
    // just a safeguard for validation, leave this first
    kCenterDetectionMode_Min = 0,

    kCenterDetectionMode_InPhase = 0,
    kCenterDetectionMode_InversePhaseLeft,
    kCenterDetectionMode_InversePhaseRight,
    kCenterDetectionMode_BothPhaseLeft,
    kCenterDetectionMode_BothPhaseRight,

    // just a safeguard for validation, leave this last
    kCenterDetectionMode_Max
};

}
}
