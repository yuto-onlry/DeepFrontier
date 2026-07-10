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
    forward = VGet(0.0f, 0.0f, 1.0f);
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
void Player::Update(const InputManager& inputManager, VECTOR cameraForward, VECTOR cameraRight, bool isLockOn, VECTOR lockOnTargetPos)
{
    velocity = VGet(0.0f, 0.0f, 0.0f);

    float walkSpeed = 5.0f;
    float dashSpeed = 9.0f;

    VECTOR moveInput = inputManager.GetLeftStick();
    weapon.SetAttackColliderActive(false);

    // カメラ方向をXZ平面だけで使う
    cameraForward.y = 0.0f;
    cameraRight.y = 0.0f;
    // カメラ前方向の正規化
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

    // カメラ右方向の正規化
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

    // カメラ基準の移動方向
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
    }    MV1SetPosition(modelHandle, position);
    UpdateCollider();

    weapon.Update();
    //アニメーションの更新
    if (playerAction.IsAction())
    {
        // ロックオン中はターゲットの方向を向く
        if (isLockOn == true)
            LookAtTarget(lockOnTargetPos);

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
        if (isLockOn == true)
            LookAtTarget(lockOnTargetPos);
        
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
            if (isLockOn == false)
            {
                forward = moveDir;
            }
            else
            {
                LookAtTarget(lockOnTargetPos);
            }
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
        animationManager.ChangeAnim(AnimationType::Jump);
    }

    // 移動速度
    float speed = walkSpeed;

    if (inputManager.IsButton(InputManager::PadButton::LB) && isMove == true)
    {
        speed = dashSpeed;
    }

    velocity.x = moveDir.x * speed;
    velocity.z = moveDir.z * speed;

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

            animationManager.ChangeAnim(AnimationType::Jump);
        }
        else
            animationManager.ChangeAnim(AnimationType::Jump);
    }
    else
    {
        if (isLockOn == true)
        {
            // ロックオン中は常に敵の方向を向く
            LookAtTarget(lockOnTargetPos);

            if (isMove == true)
            {
                state = PlayerState::Walk;

                // 左右入力が強いなら横移動アニメーション
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

    // ロックオン中は敵の方向を向く
    if (isLockOn == true)
    {
        LookAtTarget(lockOnTargetPos);
    }
    // 通常時は移動方向を向く
    else if (isMove == true)
    {
        forward = moveDir;

        float angleY = atan2f(forward.x, forward.z);
        float modelOffset = DX_PI_F;

        MV1SetRotationXYZ(
            modelHandle,
            VGet(0.0f, angleY + modelOffset, 0.0f));
    } 
    MV1SetPosition(modelHandle, position);
    UpdateCollider();
    weapon.Update();
}
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
/// <summary>
/// 指定した座標の方向を向く
/// </summary>
/// <param name="targetPos"></param>
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

    // 攻撃方向にも使えるように forward を更新
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