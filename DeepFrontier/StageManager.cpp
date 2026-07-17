#include "StageManager.h"
#include <cmath>
#include <cstdio>

namespace
{
    void DrawCircleXZ(VECTOR center, float radius, int divNum, int color)
    {
        const float PI = 3.1415926535f;

        for (int i = 0; i < divNum; i++)
        {
            float angle1 = 2.0f * PI * i / divNum;
            float angle2 = 2.0f * PI * (i + 1) / divNum;

            VECTOR p1 = VGet(
                center.x + cosf(angle1) * radius,
                center.y,
                center.z + sinf(angle1) * radius
            );

            VECTOR p2 = VGet(
                center.x + cosf(angle2) * radius,
                center.y,
                center.z + sinf(angle2) * radius
            );

            DrawLine3D(p1, p2, color);
        }
    }
}
StageManager::StageManager()
    : stageModelHandle(-1),
    position(VGet(0.0f, 0.0f, 0.0f)),
    scale(VGet(1.0f, 1.0f, 1.0f)),
    rotation(VGet(0.0f, 0.0f, 0.0f)),
    stageCenter(VGet(0.0f, 0.0f, 0.0f)),
    stageRadius(0.0f),
    groundY(0.0f)
{
}

StageManager::~StageManager()
{
    Release();
}

bool StageManager::Init()
{
    stageModelHandle = MV1LoadModel("../3dModel/Stage/Arena.mv1");

    if (stageModelHandle == -1)
    {
        return false;
    }

    position = VGet(0.0f, 0.0f, 0.0f);
    scale = VGet(0.1f, 0.1f, 0.1f);
    rotation = VGet(0.0f, 0.0f, 0.0f);

    // ステージ判定用
    stageCenter = VGet(0.0f, 0.0f, 0.0f);
    stageRadius =2200.0f;
    groundY = 0.0f;

    // ステージの当たり判定情報を作成
    //MV1SetupCollInfo(stageModelHandle, -1, 8, 4, 8);

    MV1SetPosition(stageModelHandle, position);
    MV1SetScale(stageModelHandle, scale);
    MV1SetRotationXYZ(stageModelHandle, rotation);

    return true;
}

void StageManager::Update()
{
    if (stageModelHandle == -1)
    {
        return;
    }

    MV1SetPosition(stageModelHandle, position);
    MV1SetScale(stageModelHandle, scale);
    MV1SetRotationXYZ(stageModelHandle, rotation);
}

void StageManager::Draw()
{
    if (stageModelHandle == -1)
    {
        return;
    }

    MV1DrawModel(stageModelHandle);

    // デバッグ用：ステージ範囲
    DrawCircleXZ(
        VGet(stageCenter.x, groundY + 5.0f, stageCenter.z),
        stageRadius,
        64,
        GetColor(255, 255, 0)
    );
}

void StageManager::Release()
{

    if (stageModelHandle != -1)
    {

        // いったんコメントアウトしているなら、このままでOK
        // MV1TerminateCollInfo(stageModelHandle, -1);


        MV1DeleteModel(stageModelHandle);

        stageModelHandle = -1;
    }

}
VECTOR StageManager::ClampPosition(VECTOR targetPosition, float radius) const
{
    VECTOR result = targetPosition;

    // 床より下に行かない
    if (result.y < groundY)
    {
        result.y = groundY;
    }

    float dx = result.x - stageCenter.x;
    float dz = result.z - stageCenter.z;

    float distanceSq = dx * dx + dz * dz;

    float limitRadius = stageRadius - radius;

    if (distanceSq > limitRadius * limitRadius)
    {
        float distance = sqrtf(distanceSq);

        if (distance > 0.001f)
        {
            dx /= distance;
            dz /= distance;

            result.x = stageCenter.x + dx * limitRadius;
            result.z = stageCenter.z + dz * limitRadius;
        }
    }

    return result;
}

/// <summary>
/// 
/// </summary>
/// <param name="cameraPosition"></param>
/// <returns></returns>
VECTOR StageManager::ClampCameraPosition(VECTOR cameraPosition) const
{
    VECTOR result = cameraPosition;

    float dx = result.x - stageCenter.x;
    float dz = result.z - stageCenter.z;

    float distanceSq = dx * dx + dz * dz;

    // カメラは壁より少し内側
    float cameraLimitRadius = stageRadius - 150.0f;

    if (distanceSq > cameraLimitRadius * cameraLimitRadius)
    {
        float distance = sqrtf(distanceSq);

        if (distance > 0.001f)
        {
            dx /= distance;
            dz /= distance;

            result.x = stageCenter.x + dx * cameraLimitRadius;
            result.z = stageCenter.z + dz * cameraLimitRadius;
        }
    }

    return result;
}

bool StageManager::GetGroundYByRay(VECTOR targetPosition,float& outGroundY) const
{
    if (stageModelHandle == -1)
    {
        return false;
    }

    VECTOR start = VGet(
        targetPosition.x,
        targetPosition.y + 1000.0f,
        targetPosition.z
    );

    VECTOR end = VGet(
        targetPosition.x,
        targetPosition.y - 1000.0f,
        targetPosition.z
    );

    MV1_COLL_RESULT_POLY hitResult = MV1CollCheck_Line(stageModelHandle, -1, start, end);

    if (hitResult.HitFlag == TRUE)
    {
        outGroundY = hitResult.HitPosition.y;
        return true;
    }

    return false;
}

VECTOR StageManager::FitPositionToGround(
    VECTOR targetPosition,
    float footOffset
) const
{
    VECTOR result = targetPosition;

    float hitGroundY = 0.0f;

    if (GetGroundYByRay(result, hitGroundY) == true)
    {
        result.y = hitGroundY + footOffset;
    }
    else
    {
        result.y = groundY + footOffset;
    }

    return result;
}

float StageManager::GetGroundY() const
{
    return groundY;
}