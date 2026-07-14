#include "DxLib.h"
#include "Player.h"
#include "AnimationPreset.h"
#include "ModelPreset.h"
#include <cmath>

namespace
{
    const float PLAYER_MODEL_ROT_X = 0.0f;
    const float PLAYER_MODEL_OFFSET_Y = DX_PI_F;
}

Player::Player()
    : state(PlayerState::Idle),
    animModelHandle(-1),
    verticalVelocity(0.0f),
    isJumping(false),
    playerAction(),
    forward(VGet(0.0f, 0.0f, 1.0f)),
    weapon(),
    isAttackHit(false)
{
}

Player::~Player()
{
}

void Player::Init()
{
    maxHp = 100;
    hp = maxHp;
    attack = 10;
    defense = 2;

    position = VGet(0.0f, 0.0f, 0.0f);
    velocity = VGet(0.0f, 0.0f, 0.0f);
    forward = VGet(0.0f, 0.0f, 1.0f);

    modelHandle = ModelPreset::LoadPlayerModel();
    if (modelHandle == -1)
    {
        return;
    }

    rootMotion.Init(modelHandle);
    MV1SetPosition(modelHandle, position);

    AnimationPreset::SetAnimationPlayer(animationManager, modelHandle);

    capsuleCollider.SetTag(ColliderTag::Player);
    capsuleCollider.SetOwner(this);
    capsuleCollider.SetRadius(50.0f);
    capsuleCollider.SetHeight(310.0f);
    capsuleCollider.SetActive(true);

    UpdateCollider();

    weapon.Init(modelHandle, this);

    playerAction.Init(this);
    playerCombo.Init();
    isAttackHit = false;
    isDead = false;

}

void Player::Update(
    const InputManager& inputManager,
    VECTOR cameraForward,
    VECTOR cameraRight,
    bool isLockOn,
    VECTOR lockOnTargetPos
)
{
    velocity = VGet(0.0f, 0.0f, 0.0f);

    const float walkSpeed = 5.0f;
    const float dashSpeed = 9.0f;

    VECTOR moveInput = inputManager.GetLeftStick();

    weapon.SetAttackColliderActive(false);

    cameraForward.y = 0.0f;
    cameraRight.y = 0.0f;

    float forwardLength = sqrtf(
        cameraForward.x * cameraForward.x +
        cameraForward.z * cameraForward.z
    );

    if (forwardLength > 0.0f)
    {
        cameraForward.x /= forwardLength;
        cameraForward.z /= forwardLength;
    }
    else
    {
        cameraForward = VGet(0.0f, 0.0f, 1.0f);
    }

    float rightLength = sqrtf(
        cameraRight.x * cameraRight.x +
        cameraRight.z * cameraRight.z
    );

    if (rightLength > 0.0f)
    {
        cameraRight.x /= rightLength;
        cameraRight.z /= rightLength;
    }
    else
    {
        cameraRight = VGet(1.0f, 0.0f, 0.0f);
    }

    VECTOR moveDir = VGet(0.0f, 0.0f, 0.0f);

    moveDir.x = cameraRight.x * moveInput.x + cameraForward.x * moveInput.z;
    moveDir.z = cameraRight.z * moveInput.x + cameraForward.z * moveInput.z;
    moveDir.y = 0.0f;

    bool isMove = false;

    float moveLength = sqrtf(
        moveDir.x * moveDir.x +
        moveDir.z * moveDir.z
    );

    if (moveLength > 0.0f)
    {
        moveDir.x /= moveLength;
        moveDir.z /= moveLength;
        isMove = true;
    }

    // アクション中
    if (playerAction.IsAction())
    {
        bool wasAttack = playerAction.IsAttack();

        if (wasAttack == true && inputManager.IsButtonDown(InputManager::PadButton::X) &&
            playerAction.IsComboAcceptFrame() == true)
        {
            playerCombo.RequestNext();
        }

        if (isLockOn == true)
        {
            LookAtTarget(lockOnTargetPos);
        }

        playerAction.Update(position, forward);

        // 攻撃が終わった瞬間
        if (wasAttack == true && playerAction.IsAttack() == false)
        {
            rootMotion.End(modelHandle, position);

            weapon.SetAttackColliderActive(false);
            isAttackHit = true;

            if (playerCombo.ShouldNext() == true)
            {
                StartComboAttack(
                    playerCombo.GetNextIndex(),
                    isLockOn,
                    lockOnTargetPos
                );

                animationManager.Update();

                MV1SetPosition(modelHandle, position);
                UpdateCollider();
                weapon.Update();

                return;
            }

            playerCombo.Reset();

            state = PlayerState::Idle;

            if (isLockOn == true)
            {
                animationManager.ChangeAnim(AnimationType::LockOnIdle);
            }
            else
            {
                animationManager.ChangeAnim(AnimationType::Idle);
            }

            animationManager.Update();

            MV1SetPosition(modelHandle, position);
            UpdateCollider();
            weapon.Update();

            return;
        }

        animationManager.Update();

        if (state == PlayerState::Attack && rootMotion.IsActive() == true)
        {
            rootMotion.Update(modelHandle, position);
        }
        else
        {
            MV1SetPosition(modelHandle, position);
        }

        UpdateCollider();
        weapon.Update();

        if (state == PlayerState::Attack && isAttackHit == false)
        {
            weapon.SetAttackColliderActive(true);
        }
        else
        {
            weapon.SetAttackColliderActive(false);
        }

        return;
    }

    // 攻撃
    if (inputManager.IsButtonDown(InputManager::PadButton::X))
    {
        StartComboAttack(0, isLockOn, lockOnTargetPos);

        animationManager.Update();

        UpdateCollider();
        weapon.Update();

        return;
    }

    // 回避
    if (inputManager.IsButtonDown(InputManager::PadButton::B))
    {
        state = PlayerState::Avoid;
        animationManager.ChangeAnim(AnimationType::Avoid);

        if (isMove == true)
        {
            position.x += moveInput.x * 80.0f;
            position.z += moveInput.z * 80.0f;

            if (isLockOn == false)
            {
                forward = moveDir;
            }
            else
            {
                LookAtTarget(lockOnTargetPos);
            }
        }

        weapon.SetAttackColliderActive(false);

        animationManager.Update();

        MV1SetPosition(modelHandle, position);
        UpdateCollider();
        weapon.Update();

        return;
    }

    // ジャンプ
    if (inputManager.IsButtonDown(InputManager::PadButton::A) && isJumping == false)
    {
        isJumping = true;
        verticalVelocity = 15.0f;

        state = PlayerState::Jump;
        animationManager.ChangeAnim(AnimationType::Jump);
    }

    float speed = walkSpeed;

    if (inputManager.IsButton(InputManager::PadButton::LB) && isMove == true)
    {
        speed = dashSpeed;
    }

    velocity.x = moveDir.x * speed;
    velocity.z = moveDir.z * speed;

    position = VAdd(position, velocity);

    if (isJumping == true)
    {
        position.y += verticalVelocity;
        verticalVelocity -= 0.8f;

        if (position.y <= 0.0f)
        {
            position.y = 0.0f;
            verticalVelocity = 0.0f;
            isJumping = false;

            animationManager.ChangeAnim(AnimationType::Jump);
        }
        else
        {
            animationManager.ChangeAnim(AnimationType::Jump);
        }
    }
    else
    {
        if (isLockOn == true)
        {
            LookAtTarget(lockOnTargetPos);

            if (isMove == true)
            {
                state = PlayerState::Walk;

                if (fabsf(moveInput.x) > fabsf(moveInput.z))
                {
                    animationManager.ChangeAnim(AnimationType::LockOnLateralMove);
                }
                else
                {
                    animationManager.ChangeAnim(AnimationType::LockOnWalk);
                }
            }
            else
            {
                state = PlayerState::Idle;
                animationManager.ChangeAnim(AnimationType::LockOnIdle);
            }
        }
        else
        {
            if (isMove == true)
            {
                if (inputManager.IsButton(InputManager::PadButton::LB))
                {
                    state = PlayerState::Run;
                    animationManager.ChangeAnim(AnimationType::Run);
                }
                else
                {
                    state = PlayerState::Walk;
                    animationManager.ChangeAnim(AnimationType::Walk);
                }
            }
            else
            {
                state = PlayerState::Idle;
                animationManager.ChangeAnim(AnimationType::Idle);
            }
        }
    }

    animationManager.Update();

    if (isLockOn == true)
    {
        LookAtTarget(lockOnTargetPos);
    }
    else if (isMove == true)
    {
        forward = moveDir;

        float angleY = atan2f(forward.x, forward.z);
        float modelOffset = DX_PI_F;

        MV1SetRotationXYZ(
            modelHandle,
            VGet(0.0f, angleY + modelOffset, 0.0f)
        );
    }

    MV1SetPosition(modelHandle, position);
    UpdateCollider();
    weapon.Update();
}

void Player::StartComboAttack(int index, bool isLockOn, VECTOR lockOnTargetPos)
{
    if (isLockOn == true)
    {
        LookAtTarget(lockOnTargetPos);
    }

    state = PlayerState::Attack;

    playerCombo.SetIndex(index);

    AnimationType animType = playerCombo.GetAnimationType(isLockOn);
    animationManager.ChangeAnim(animType);

    PlayerAction::AttackData attackData = playerCombo.GetAttackData();
    playerAction.StartAttack(position, forward, attackData);

    isAttackHit = false;

    rootMotion.Start(modelHandle, position);
    
    weapon.SetAttackColliderActive(true);
}

void Player::UpdateCollider()
{
    if (playerAction.IsAttack() == true)
    {
        VECTOR hipsPos = rootMotion.GetHipsWorldPosition(modelHandle, position);

        capsuleCollider.SetPosition(
            VGet(
                hipsPos.x,
                position.y + 200.0f,
                hipsPos.z
            )
        );

        return;
    }

    capsuleCollider.SetPosition(
        VGet(position.x, position.y + 200.0f, position.z)
    );
}

void Player::Draw()
{
    if (modelHandle != -1)
    {
        CharacterBase::Draw();
    }

    weapon.Draw();
}

void Player::Release()
{
    weapon.Release();
    animationManager.Release();

    if (animModelHandle != -1)
    {
        MV1DeleteModel(animModelHandle);
        animModelHandle = -1;
    }

    CharacterBase::Release();
}

void Player::LookAtTarget(VECTOR targetPos)
{
    VECTOR direction;

    direction.x = targetPos.x - position.x;
    direction.y = 0.0f;
    direction.z = targetPos.z - position.z;

    float length = sqrtf(
        direction.x * direction.x +
        direction.z * direction.z
    );

    if (length <= 0.001f)
    {
        return;
    }

    direction.x /= length;
    direction.z /= length;

    forward = direction;

    if (modelHandle != -1)
    {
        float angleY = atan2f(forward.x, forward.z);

        MV1SetRotationXYZ(
            modelHandle,
            VGet(
                PLAYER_MODEL_ROT_X,
                angleY + PLAYER_MODEL_OFFSET_Y,
                0.0f
            )
        );
    }
}
/// <summary>
/// コンボのインデックスを取得
/// </summary>
/// <returns></returns>
int Player::GetComboIndex() const
{
    return playerCombo.GetIndex();
}

int Player::GetAttackColliderCount() const
{
    return weapon.GetAttackColliderCount();
}

SphereCollider* Player::GetAttackCollider(int index)
{
    return weapon.GetAttackCollider(index);
}

void Player::DisableAttackCollider()
{
    isAttackHit = true;
    weapon.SetAttackColliderActive(false);
}