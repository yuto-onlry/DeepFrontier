#pragma once

#include "CharacterBase.h"
#include "animationManager.h"
#include "InputManager.h"
#include "PlayerAction.h"
#include "SphereCollider.h"
#include "Weapon.h"
#include "PlayerRootMotion.h"
#include "PlayerCombo.h"
#include <dinput.h>

class Player : public CharacterBase
{
public:
    enum class PlayerState
    {
        Idle,
        Walk,
        Run,
        Jump,
        Attack,
        Avoid,
        Damage,
        GetUp,
        Dead
    };

public:
    DINPUT_JOYSTATE input;

    AnimationManager animationManager;

    PlayerState state;
    int animModelHandle;
    float verticalVelocity;
    bool isJumping;

private:
    PlayerAction playerAction;
    VECTOR forward;
    Weapon weapon;
    bool isAttackHit;

    PlayerRootMotion rootMotion;
private:
    PlayerCombo playerCombo;

public:
    Player();
    virtual ~Player();

public:
    void Init() override;

    void Update(
        const InputManager& inputManager,
        VECTOR cameraForward,
        VECTOR cameraRight,
        bool isLockOn,
        VECTOR lockOnTargetPos
    );

    void UpdateCollider();
    void Draw() override;
    void Release() override;

private:
    void LookAtTarget(VECTOR targetPos);

private:
    VECTOR GetHipsOffsetFromBase(VECTOR basePosition);
    VECTOR GetHipsWorldPosition();

public:
    int GetAttackColliderCount() const;
    SphereCollider* GetAttackCollider(int index);
public:
    int GetComboIndex() const;

public:
	// 攻撃判定を無効化する
    void DisableAttackCollider();
private:
	// 攻撃コンボを開始する
    void StartComboAttack(int index, bool isLockOn, VECTOR lockOnTargetPos);
};