#include "DxLib.h"
#include "Player.h"
#include "AnimationPreset.h"
#include "ModelPreset.h"
#include <cmath>

Player::Player()
    :state(PlayerState::Idle),
	 animModelHandle(-1),
     verticalVelocity(0.0f),
     isJumping(false),
     actionTimer(0)
{
}    
Player::~Player()
{
}

void Player::Init()
{
    hp = 100;
    attack = 10;
    defense = 2;

    position = VGet(0.0f, 0.0f, 0.0f);
    velocity = VGet(0.0f, 0.0f, 0.0f);
	//モデルの読み込み
	modelHandle = ModelPreset::LoadPlayerModel();
    if (modelHandle == -1)
        return;
	//モデルの位置をセット
    MV1SetPosition(modelHandle, position);
	//アニメーションのセット
    AnimationPreset::SetAnimationPlayer(animationManager, modelHandle);

    bodyCollider.SetTag(ColliderTag::Player);
    bodyCollider.SetOwner(this);
    bodyCollider.SetRadius(80.0f);
    bodyCollider.SetPosition(VGet(position.x, position.y + 100.0f, position.z));
    bodyCollider.SetActive(true);

    isDead = false;
}
void Player::Update(const InputManager& inputManager)
{
    velocity = VGet(0.0f, 0.0f, 0.0f);

    float walkSpeed = 5.0f;
    float dashSpeed = 9.0f;

    VECTOR moveInput = inputManager.GetLeftStick();

    bool isMove = false;

    if (moveInput.x != 0.0f || moveInput.z != 0.0f)
    {
        isMove = true;
    }

    // 攻撃・回避中は一定時間、通常移動アニメーションで上書きしない
    if (actionTimer > 0)
    {
        actionTimer--;

        animationManager.Update();
        MV1SetPosition(modelHandle, position);
        return;
    }

    // Xボタン：攻撃
    if (inputManager.IsButtonDown(InputManager::PadButton::X))
    {
        state = PlayerState::Attack;
        animationManager.ChangeAnim(AnimationType::Attack);

        actionTimer = 30;
        animationManager.Update();
        MV1SetPosition(modelHandle, position);
        return;
    }

    // Bボタン：回避
    if (inputManager.IsButtonDown(InputManager::PadButton::B))
    {
        state = PlayerState::Avoid;
        animationManager.ChangeAnim(AnimationType::Avoid);

        // 入力方向へ少し回避移動
        if (isMove == true)
        {
            position.x += moveInput.x * 80.0f;
            position.z += moveInput.z * 80.0f;
        }

        actionTimer = 20;
        animationManager.Update();
        MV1SetPosition(modelHandle, position);
        return;
    }

    // Aボタン：ジャンプ
    if (inputManager.IsButtonDown(InputManager::PadButton::A) && isJumping == false)
    {
        isJumping = true;
        verticalVelocity = 15.0f;

        state = PlayerState::Jump;
        animationManager.ChangeAnim(AnimationType::JumpStart);
    }

    // 移動速度
    float speed = walkSpeed;

    if (inputManager.IsButton(InputManager::PadButton::LB) && isMove == true)
    {
        speed = dashSpeed;
    }

    velocity.x = moveInput.x * speed;
    velocity.z = moveInput.z * speed;

    position = VAdd(position, velocity);

    // ジャンプ処理
    if (isJumping == true)
    {
        position.y += verticalVelocity;
        verticalVelocity -= 0.8f;

        if (position.y <= 0.0f)
        {
            position.y = 0.0f;
            verticalVelocity = 0.0f;
            isJumping = false;

            animationManager.ChangeAnim(AnimationType::JumpEnd);
        }
        else
        {
            animationManager.ChangeAnim(AnimationType::JumpLoop);
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

    animationManager.Update();

    // 移動しているときだけ向きを変える
    if (isMove == true)
    {
        float angleY = atan2f(velocity.x, velocity.z);
        float modelOffset = DX_PI_F;

        MV1SetRotationXYZ(
            modelHandle,
            VGet(DX_PI_F / 2.0f, angleY + modelOffset, 0.0f)
        );
    }

    MV1SetPosition(modelHandle, position);
    bodyCollider.SetPosition(VGet(position.x, position.y + 100.0f, position.z));
}void Player::Draw()
{
    if (modelHandle != -1)
        CharacterBase::Draw();
    else
        //球体
        DrawSphere3D(position, 30.0f, 16, GetColor(0, 255, 0), GetColor(0, 255, 0), TRUE);
}

void Player::Release()
{
    animationManager.Release();

    if (animModelHandle != -1)
    {
        MV1DeleteModel(animModelHandle);
        animModelHandle = -1;
    }

    CharacterBase::Release();
}
