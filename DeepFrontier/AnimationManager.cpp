#include "AnimationManager.h"

AnimationManager::AnimationManager()
    : modelHandle(-1),
    animSrcHandle(-1),
    currentAnimType(AnimationType::Max),
    currentAnimIndex(-1),
    attachAnimIndex(-1),
    animTime(0.0f),
    animTotalTime(0.0f),
    animSpeed(0.5f)
{
    for (int i = 0; i < (int)AnimationType::Max; i++)
    {
        animNoTable[i] = -1;
        animSrcHandleTable[i] = -1;
    }
}


AnimationManager::~AnimationManager()
{
   
}

void AnimationManager::Init(int model)
{
    modelHandle = model;
    animSrcHandle = -1;

    currentAnimType = AnimationType::Max;
    currentAnimIndex = -1;
    attachAnimIndex = -1;

    animTime = 0.0f;
    animTotalTime = 0.0f;
    animSpeed = 0.5f;

    for (int i = 0; i < (int)AnimationType::Max; i++)
    {
        animNoTable[i] = -1;
        animSrcHandleTable[i] = -1;
    }
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
    int typeIndex = (int)type;

    if (typeIndex < 0 || typeIndex >= (int)AnimationType::Max)
    {
        return;
    }

    int animIndex = animNoTable[typeIndex];

    if (animIndex == -1)
    {
        return;
    }
    if (currentAnimType == type)
    {
        return;
    }

    int useAnimSrcHandle = animSrcHandle;

    // TypeごとのアニメMV1がある場合はこちらを優先
    if (animSrcHandleTable[typeIndex] != -1)
    {
        useAnimSrcHandle = animSrcHandleTable[typeIndex];
    }

    if (useAnimSrcHandle == -1)
    {
        return;
    }

    if (attachAnimIndex != -1)
    {
        MV1DetachAnim(modelHandle, attachAnimIndex);
        attachAnimIndex = -1;
    }

    attachAnimIndex = MV1AttachAnim(modelHandle, animIndex, useAnimSrcHandle, TRUE);

    if (attachAnimIndex == -1)
    {
        return;
    }

    currentAnimType = type;
    currentAnimIndex = animIndex;

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
bool AnimationManager::LoadAnimModelForType(AnimationType type, const char* filePath, int animIndex)
{
    int typeIndex = (int)type;

    if (typeIndex < 0 || typeIndex >= (int)AnimationType::Max)
    {
        return false;
    }

    int handle = MV1LoadModel(filePath);

    if (handle == -1)
    {
        return false;
    }

    animSrcHandleTable[typeIndex] = handle;
    animNoTable[typeIndex] = animIndex;

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

    for (int i = 0; i < (int)AnimationType::Max; i++)
    {
        if (animSrcHandleTable[i] != -1)
        {
            MV1DeleteModel(animSrcHandleTable[i]);
            animSrcHandleTable[i] = -1;
        }
    }

    modelHandle = -1;
    currentAnimType = AnimationType::Max;
    currentAnimIndex = -1;
    attachAnimIndex = -1;

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