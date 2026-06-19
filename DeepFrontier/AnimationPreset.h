#pragma once
#include "AnimationManager.h"

class AnimationPreset
{
public:
    static void SetAnimationPlayer(AnimationManager& animManager, int modelHandle);
    static void SetAnimationLittleEnemy(AnimationManager& animManager, int modelHandle);
};