#include "AnimationManager.h"

AnimationManager::AnimationManager()
    :modelHandle(-1),
	animSrcHandle(-1),
    currentAnimIndex(-1),
    attachAnimIndex(-1),
    animTime(0.0f),
    animTotalTime(0.0f),
    animSpeed(0.5f)
{
    for (int i = 0; i < (int)AnimationType::Max; i++)
    {
        animNoTable[i] = -1;
    }
}


AnimationManager::~AnimationManager()
{
    Release();
}

void AnimationManager::Init(int model)
{
    modelHandle = model;
    animSrcHandle = -1;

    currentAnimIndex = -1;
    attachAnimIndex = -1;

    animTime = 0.0f;
    animTotalTime = 0.0f;
    animSpeed = 0.5f;

    for (int i = 0; i < (int)AnimationType::Max; i++)
        animNoTable[i] = -1;
}
void AnimationManager::Update()
{
    if (modelHandle == -1)
    {
        return;
    }

    if (attachAnimIndex == -1)
    {
        return;
    }

    animTime += animSpeed;

    if (animTime >= animTotalTime)
    {
        animTime = 0.0f;
    }

    MV1SetAttachAnimTime(modelHandle, attachAnimIndex, animTime);
}

void AnimationManager::ChangeAnim(AnimationType type)
{
    if (animSrcHandle == -1)
        return;

    int animindex = animNoTable[(int)type];

    if (animindex == -1)
        return;

    if (currentAnimIndex == animindex)
        return;

    if (attachAnimIndex != -1)
    {
        MV1DetachAnim(modelHandle, attachAnimIndex);
        attachAnimIndex = -1;
    }

    attachAnimIndex = MV1AttachAnim(modelHandle, animindex, animSrcHandle, TRUE);

    if (attachAnimIndex == -1)
        return;

    currentAnimIndex = animindex;
    animTime = 0.0f;
    animTotalTime = MV1GetAttachAnimTotalTime(modelHandle, attachAnimIndex);

}
bool AnimationManager::LoadAnimModel(const char* filePath)
{
    animSrcHandle = MV1LoadModel(filePath);

    if (animSrcHandle == -1)
        return false;
    return true;
}

void AnimationManager::Release()
{
    if (modelHandle != -1 && attachAnimIndex != -1)
    {
        MV1DetachAnim(modelHandle, attachAnimIndex);
        attachAnimIndex = -1;
    }

    if (animSrcHandle != -1)
    {
        MV1DeleteModel(animSrcHandle);
        animSrcHandle = -1;
    }

    currentAnimIndex = -1;
    animTime = 0.0f;
    animTotalTime = 0.0f;
}
void AnimationManager::SetAnimIndex(AnimationType type, int animIndex)
{
    animNoTable[(int)type] = animIndex;
}

void AnimationManager::SetSpeed(float speed)
{
    animSpeed = speed;
}