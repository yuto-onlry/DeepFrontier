#pragma once
#include "DxLib.h"
#include "SphereCollider.h"

class Weapon
{
private:
    // 武器モデルのハンドル
    int WeaponModelHandle;
    // 武器を装備しているキャラクターのモデルハンドル
    int ownerModelHandle;
    int attachFrameIndex;

    VECTOR offsetPosition;
    VECTOR offsetRotation;
    VECTOR scale;
private:
    static const int AttackColliderCount = 5;
    // 攻撃判定用のコライダー
    SphereCollider attackColliders[AttackColliderCount];
    // 攻撃判定の位置
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
    int GetAttackColliderCount() const;
    SphereCollider* GetAttackCollider(int index);
public:
    void SetAttackColliderActive(bool active);
public:
    bool IsValid() const;
};