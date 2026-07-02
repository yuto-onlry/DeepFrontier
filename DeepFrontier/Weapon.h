#pragma once
#include "DxLib.h"

class Weapon
{
private:
    int modelHandle;

    int ownerModelHandle;
    int attachFrameIndex;

    VECTOR offsetPosition;
    VECTOR offsetRotation;
    VECTOR scale;

public:
    Weapon();
    ~Weapon();

public:
    bool Init(int playerModelHandle);
    void Update();
    void Draw();
    void Release();

public:
    bool IsValid() const;
};