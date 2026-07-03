#include "DxLib.h"
#include "Player.h"
#include "AnimationPreset.h"
#include "ModelPreset.h"
#include <cmath>

Player::Player()
    :state(PlayerState::Idle),
	 animModelHandle(-1),
     verticalVelocity(0.0f),
     isJumping(false)
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
	//モデルの読み込み
	modelHandle = ModelPreset::LoadPlayerModel();
    if (modelHandle == -1)
        return;
	//モデルの位置をセット
    MV1SetPosition(modelHandle, position);
	//アニメーションのセット
    AnimationPreset::SetAnimationPlayer(animationManager, modelHandle);
    //コライダーセット
    capsuleCollider.SetTag(ColliderTag::Player);
    capsuleCollider.SetOwner(this);
    capsuleCollider.SetRadius(50.0f);
    capsuleCollider.SetHeight(310.0f);
    capsuleCollider.SetActive(true);
    UpdateCollider();
    weapon.Init(modelHandle, this); 
    //アクションコライダー
    playerAction.Init(this);
    isDead = false;
}
void Player::Update(const InputManager& inputManager)
{
    velocity = VGet(0.0f, 0.0f, 0.0f);

    float walkSpeed = 5.0f;
    float dashSpeed = 9.0f;

    VECTOR moveInput = inputManager.GetLeftStick();
    weapon.SetAttackColliderActive(false);

    bool isMove = false;

    if (moveInput.x != 0.0f || moveInput.z != 0.0f)
    {
        isMove = true;
    }
    if (CheckHitKey(KEY_INPUT_H))
    {
		Player::Damage(10);
    }
    MV1SetPosition(modelHandle, position);
    UpdateCollider();

    weapon.Update();
    //アニメーションの更新
    if (playerAction.IsAction())
    {
        playerAction.Update(position, forward);
        // 攻撃中、かつまだヒットしていない時だけ武器判定true
        if (state == PlayerState::Attack && isAttackHit == false)
        {
            weapon.SetAttackColliderActive(true);
        }        animationManager.Update();
        MV1SetPosition(modelHandle, position);
        UpdateCollider();
        weapon.Update();

        return;
    }
    //攻撃
    if (inputManager.IsButtonDown(InputManager::PadButton::X))
    {
        state = PlayerState::Attack;
        animationManager.ChangeAnim(AnimationType::Attack);

        playerAction.StartAttack(position, forward);
        // 新しい攻撃なので、まだ当たっていない状態に戻す
        isAttackHit = false;
		// 攻撃開始フレームから攻撃判定をtrue
        weapon.SetAttackColliderActive(true);

        animationManager.Update();
        MV1SetPosition(modelHandle, position);
        UpdateCollider();
        weapon.Update();

        return;
    }
    //回避
    if (inputManager.IsButtonDown(InputManager::PadButton::B))
    {
        state = PlayerState::Avoid;
        animationManager.ChangeAnim(AnimationType::Avoid);

        if (isMove == true)
        {
            position.x += moveInput.x * 80.0f;
            position.z += moveInput.z * 80.0f;
        }
        // 回避では攻撃判定を出さない
        weapon.SetAttackColliderActive(false);
        MV1SetPosition(modelHandle, position);
		// 回避中のコライダー更新
        UpdateCollider();
		// 回避中のアニメーション更新
        animationManager.Update();
		// 回避中の武器の更新
        weapon.Update();

        return;
    }
    //ジャンプ
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
            animationManager.ChangeAnim(AnimationType::JumpLoop);
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
        float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
        if (length > 0.0f)
        {
            forward.x = velocity.x / length;
            forward.y = 0.0f;
            forward.z = velocity.z / length;
        }
        MV1SetRotationXYZ(modelHandle,VGet(DX_PI_F / 2.0f, angleY + modelOffset, 0.0f));
    }
     
    MV1SetPosition(modelHandle, position);
    UpdateCollider();
    weapon.Update();}
void Player::UpdateCollider()
{
	//プレイヤーコライダーの位置を更新
    capsuleCollider.SetPosition(VGet(position.x, position.y + 200.0f, position.z));
}
void Player::Draw()
{
    if (modelHandle != -1)
        CharacterBase::Draw();

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
// 攻撃判定のコライダーの数を取得
int Player::GetAttackColliderCount() const
{
    return weapon.GetAttackColliderCount();
}

// 攻撃判定のコライダーを取得
SphereCollider* Player::GetAttackCollider(int index)
{
    return weapon.GetAttackCollider(index);
}// 攻撃判定のコライダーを無効化
void Player::DisableAttackCollider()
{
	isAttackHit = true;
    weapon.SetAttackColliderActive(false);
}