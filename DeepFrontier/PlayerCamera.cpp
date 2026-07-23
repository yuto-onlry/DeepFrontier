#include "PlayerCamera.h"
#include <cmath>

static VECTOR LerpVector(VECTOR current, VECTOR target, float t)
{
    return VGet(
        current.x + (target.x - current.x) * t,
        current.y + (target.y - current.y) * t,
        current.z + (target.z - current.z) * t
    );
}

PlayerCamera::PlayerCamera()
    : position(VGet(0.0f, 500.0f, -800.0f)),
    target(VGet(0.0f, 150.0f, 0.0f)),
    distance(800.0f),
    height(450.0f),
    targetHeight(180.0f),
    followSpeed(0.08f),
    angleY(0.0f),
    angleX(0.25f),
    rotateSpeed(0.03f),
    smoothPlayerPosition(VGet(0.0f, 0.0f, 0.0f)),
    isSmoothPlayerPositionInit(false)
{
}

PlayerCamera::~PlayerCamera()
{
}

VECTOR PlayerCamera::GetForward() const
{
    VECTOR forward;

    forward.x = target.x - position.x;
    forward.y = 0.0f;
    forward.z = target.z - position.z;

    float length = sqrtf(forward.x * forward.x + forward.z * forward.z);

    if (length <= 0.001f)
    {
        return VGet(0.0f, 0.0f, 1.0f);
    }

    forward.x /= length;
    forward.z /= length;

    return forward;
}

VECTOR PlayerCamera::GetRight() const
{
    VECTOR forward = GetForward();

    return VGet(
        forward.z,
        0.0f,
        -forward.x
    );
}
void PlayerCamera::Init()
{
    position = VGet(0.0f, height, -distance);
    target = VGet(0.0f, targetHeight, 0.0f);

    Apply();
}

void PlayerCamera::Update(VECTOR playerPos,VECTOR rightStick, bool isLockOn,VECTOR lockOnTargetPos , const StageManager& stageManager)
{
    VECTOR cameraPlayerPos = SmoothPlayerPosition(playerPos);

    if (isLockOn == true)
        LockOnCamera(cameraPlayerPos, lockOnTargetPos);
    else
        NormalCamera(cameraPlayerPos, rightStick);

    // カメラをステージ内に収める
    position = stageManager.ClampCameraPosition(position);

    Apply();
}

/// <summary>
/// 通常カメラの更新
/// </summary>
/// <param name="playerPos"></param>
/// <param name="rightStick"></param>
void PlayerCamera::NormalCamera(VECTOR playerPos, VECTOR rightStick)
{
    angleY -= rightStick.x * rotateSpeed;

    angleX += rightStick.z * rotateSpeed;

    if (angleX < -0.15f)
    {
        angleX = -0.15f;
    }

    if (angleX > 0.75f)
    {
        angleX = 0.75f;
    }

    VECTOR desiredTarget = VAdd(playerPos, VGet(0.0f, targetHeight, 0.0f));

    float cosX = cosf(angleX);
    float sinX = sinf(angleX);

    VECTOR offset;

    offset.x = sinf(angleY) * distance * cosX;
    offset.y = sinX * distance + 250.0f;
    offset.z = -cosf(angleY) * distance * cosX;

    VECTOR desiredPosition = VAdd(desiredTarget, offset);

    position = LerpVector(position, desiredPosition, followSpeed);
    target = LerpVector(target, desiredTarget, followSpeed);
}

/// <summary>
/// ロックオンカメラの更新
/// </summary>
/// <param name="playerPos"></param>
/// <param name="lockOnTargetPos"></param>
void PlayerCamera::LockOnCamera(VECTOR playerPos, VECTOR lockOnTargetPos)
{
    VECTOR dir;

    dir.x = lockOnTargetPos.x - playerPos.x;
    dir.y = 0.0f;
    dir.z = lockOnTargetPos.z - playerPos.z;

    float length = sqrtf(dir.x * dir.x + dir.z * dir.z);

    if (length <= 0.001f)
    {
        return;
    }

    dir.x /= length;
    dir.z /= length;

    VECTOR right;

    right.x = dir.z;
    right.y = 0.0f;
    right.z = -dir.x;

    float lockOnDistance = 550.0f;
    float lockOnHeight = 230.0f;
    float lockOnTargetHeight = 150.0f;
	// プレイヤーの右側にカメラを配置するためのオフセット
	float sideOffset =400.0f;

    VECTOR desiredPosition;

    desiredPosition.x =
        playerPos.x
        - dir.x * lockOnDistance
        + right.x * sideOffset;

    desiredPosition.y =
        playerPos.y + lockOnHeight;

    desiredPosition.z =
        playerPos.z
        - dir.z * lockOnDistance
        + right.z * sideOffset;

    VECTOR desiredTarget;

    // プレイヤーと敵の中間を見る
    desiredTarget.x = playerPos.x * 0.45f + lockOnTargetPos.x * 0.55f;
    desiredTarget.y = playerPos.y + lockOnTargetHeight;
    desiredTarget.z = playerPos.z * 0.45f + lockOnTargetPos.z * 0.55f;

    float lockOnFollowSpeed = 0.08f;

    position = LerpVector(position, desiredPosition, lockOnFollowSpeed);
    target = LerpVector(target, desiredTarget, lockOnFollowSpeed);
}

/// <summary>
/// カメラの位置と注視点を適用する
/// </summary>
void PlayerCamera::Apply()
{
    SetCameraNearFar(1.0f, 20000.0f);
	// カメラの位置と注視点を設定
    SetCameraPositionAndTarget_UpVecY(position, target);
}

/// <summary>
/// プレイヤーの位置を滑らかに追従させる
/// </summary>
/// <param name="playerPos"></param>
/// <returns></returns>
VECTOR PlayerCamera::SmoothPlayerPosition(VECTOR playerPos)
{
    if (isSmoothPlayerPositionInit == false)
    {
        smoothPlayerPosition = playerPos;
        isSmoothPlayerPositionInit = true;
        return  smoothPlayerPosition;
    }
	// プレイヤーの位置を滑らかに追従させる
    const float smoothSpeed = 0.18f;

    smoothPlayerPosition.x += (playerPos.x - smoothPlayerPosition.x) * smoothSpeed;
    smoothPlayerPosition.y += (playerPos.y - smoothPlayerPosition.y) * smoothSpeed;
    smoothPlayerPosition.z += (playerPos.z - smoothPlayerPosition.z) * smoothSpeed;

    return smoothPlayerPosition;
}