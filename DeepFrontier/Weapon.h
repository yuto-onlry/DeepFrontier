#pragma once
#include "DxLib.h"
#include "SphereCollider.h"

class Weapon
{
private:
    int modelHandle;

    int ownerModelHandle;
    int attachFrameIndex;

    VECTOR offsetPosition;
    VECTOR offsetRotation;
    VECTOR scale;
private:
	// 攻撃判定用のコライダー
    SphereCollider attackCollider;
	// 攻撃判定の位置調整用
    VECTOR attackLocalOffset;

public:
    Weapon();
    ~Weapon();

public:
    bool Init(int playerModelHandle, CharacterBase* owner);
    void Update();
    void Draw();
    void Release();

public:
    void SetAttackColliderActive(bool active);
public:

    SphereCollider* GetAttackCollider();
public:
    bool IsValid() const;
};