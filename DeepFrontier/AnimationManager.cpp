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

void AnimationManager::Init(int model, int animSrc)
{
    modelHandle = model;
    animSrcHandle = animSrc;

    currentAnimIndex = -1;
    attachAnimIndex = -1;

    animTime = 0.0f;
    animTotalTime = 0.0f;
    animSpeed = 0.5f;

    for (int i = 0; i < (int)AnimationType::Max; i++)
    {
        animNoTable[i] = -1;
    }
}

void AnimationManager::SetAnimNo(AnimationType type, int animNo)
{
    animNoTable[(int)type] = animNo;
}

void AnimationManager::ChangeAnim(AnimationType type)
{
    if (modelHandle == -1)
    {
        return;
    }

    if (animSrcHandle == -1)
    {
        return;
    }

    int animNo = animNoTable[(int)type];

    if (animNo == -1)
    {
        return;
    }

    if (currentAnimIndex == animNo)
    {
        return;
    }

    if (attachAnimIndex != -1)
    {
        MV1DetachAnim(modelHandle, attachAnimIndex);
        attachAnimIndex = -1;
    }

    attachAnimIndex = MV1AttachAnim(modelHandle, animNo, animSrcHandle, TRUE);

    if (attachAnimIndex == -1)
    {
        printfDx("アニメーションAttach失敗 AnimNo:%d\n", animNo);
        return;
    }

    currentAnimIndex = animNo;
    animTime = 0.0f;
    animTotalTime = MV1GetAttachAnimTotalTime(modelHandle, attachAnimIndex);
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

void AnimationManager::Release()
{
    if (modelHandle != -1 && attachAnimIndex != -1)
    {
        MV1DetachAnim(modelHandle, attachAnimIndex);
        attachAnimIndex = -1;
    }

    currentAnimIndex = -1;
    animTime = 0.0f;
    animTotalTime = 0.0f;
}

void AnimationManager::SetSpeed(float speed)
{
    animSpeed = speed;
}