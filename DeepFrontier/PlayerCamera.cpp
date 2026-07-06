#include "PlayerCamera.h"
#include <cmath>

PlayerCamera::PlayerCamera()
    : position(VGet(0.0f, 500.0f, -800.0f)),
    target(VGet(0.0f, 150.0f, 0.0f)),
    distance(800.0f),
    height(450.0f),
    targetHeight(180.0f),
    followSpeed(0.08f),
    angleY(0.0f),
    angleX(0.25f),
    rotateSpeed(0.05f)
{
}

PlayerCamera::~PlayerCamera()
{
}

VECTOR PlayerCamera::GetForward() const
{
    VECTOR forward;

    // カメラの向いている方向をXZ平面だけで使う
    forward.x = -sinf(angleY);
    forward.y = 0.0f;
    forward.z = cosf(angleY);

    return VNorm(forward);
}

VECTOR PlayerCamera::GetRight() const
{
    VECTOR right;

    right.x = cosf(angleY);
    right.y = 0.0f;
    right.z = sinf(angleY);

    return VNorm(right);
}

void PlayerCamera::Init()
{
    position = VGet(0.0f, height, -distance);
    target = VGet(0.0f, targetHeight, 0.0f);

    Apply();
}

void PlayerCamera::Update(VECTOR playerPos, VECTOR rightStick)
{
    // 右スティックでカメラ回転
    angleY -= rightStick.x * rotateSpeed;

    // 上下操作
    angleX += rightStick.z * rotateSpeed;

    // 上下角度制限
    if (angleX < -0.15f)
    {
        angleX = -0.15f;
    }

    if (angleX > 0.75f)
    {
        angleX = 0.75f;
    }

    // 注視点
    VECTOR desiredTarget = VAdd(playerPos, VGet(0.0f, targetHeight, 0.0f));

    // カメラ位置計算
    float cosX = cosf(angleX);
    float sinX = sinf(angleX);

    VECTOR offset;
    offset.x = sinf(angleY) * distance * cosX;
    offset.y = sinX * distance + 250.0f;
    offset.z = -cosf(angleY) * distance * cosX;

    VECTOR desiredPosition = VAdd(desiredTarget, offset);

	// 追従
    position.x += (desiredPosition.x - position.x) * followSpeed;
    position.y += (desiredPosition.y - position.y) * followSpeed;
    position.z += (desiredPosition.z - position.z) * followSpeed;
	// 注視点の追従
    target.x += (desiredTarget.x - target.x) * followSpeed;
    target.y += (desiredTarget.y - target.y) * followSpeed;
    target.z += (desiredTarget.z - target.z) * followSpeed;

    Apply();
}

void PlayerCamera::Apply()
{
	// カメラの位置と注視点を設定
    SetCameraPositionAndTarget_UpVecY(position, target);
}