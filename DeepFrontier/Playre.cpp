#include "DxLib.h"
#include "Player.h"
#include "AnimationPreset.h"
#include "ModelPreset.h"
#include <cmath>

Player::Player()
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

    isDead = false;
}void Player::Update(const InputManager& inputManager)
{
	// 移動速度
    float speed = 5.0f;
    bool isMove = false;
    VECTOR moveInput = inputManager.GetLeftStick();

    velocity = VGet(0.0f, 0.0f, 0.0f);
    velocity.x = moveInput.x * speed;
    velocity.z = moveInput.z * speed;

    position = VAdd(position, velocity);
    if (velocity.x != 0.0f || velocity.z != 0.0f)
        isMove = true;
    
	//各動きに応じてアニメーションを切り替える
    if (isMove == true)
        animationManager.ChangeAnim(AnimationType::Walk);
    else
        animationManager.ChangeAnim(AnimationType::Idle);

    animationManager.Update();

    if (isMove == true)
    {
        float angleY = atan2f(velocity.x, velocity.z);
        float modelOffset = DX_PI_F;

        MV1SetRotationXYZ(modelHandle,VGet(DX_PI_F / 2.0f, angleY + modelOffset, 0.0f));
    }
}
void Player::Draw()
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
