#include "EnemyRootMotion.h"

EnemyRootMotion::EnemyRootMotion()
    : hipsFrameIndex(-1),
    startHipsOffset(VGet(0.0f, 0.0f, 0.0f))
{
}

void EnemyRootMotion::Init()
{
    hipsFrameIndex = -1;
    startHipsOffset = VGet(0.0f, 0.0f, 0.0f);
}

void EnemyRootMotion::Start(int modelHandle, VECTOR position)
{
    if (modelHandle == -1)
    {
        return;
    }

    MV1SetPosition(modelHandle, position);

    startHipsOffset = GetHipsOffsetFromBase(
        modelHandle,
        position
    );
}

void EnemyRootMotion::AbsorbToPosition(
    int modelHandle,
    VECTOR& position
)
{
    if (modelHandle == -1)
        return;

    VECTOR hipsPos = GetHipsWorldPosition(
        modelHandle,
        position
    );

    position.x = hipsPos.x - startHipsOffset.x;
    position.z = hipsPos.z - startHipsOffset.z;

    // 地面に固定
    position.y = 0.0f;

    MV1SetPosition(modelHandle, position);
}

/// <summary>
/// 
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="position"></param>
/// <param name="useHipsXZ"></param>
/// <returns></returns>
VECTOR EnemyRootMotion::GetBodyColliderPosition(
    int modelHandle,
    VECTOR position,
    bool useHipsXZ
)
{
    if (useHipsXZ == true)
    {
        VECTOR hipsPos = GetHipsWorldPosition(
            modelHandle,
            position
        );

        return VGet(
            hipsPos.x,
            hipsPos.y + 60.0f,
            hipsPos.z
        );
    }

    return VGet(
        position.x,
        position.y + 150.0f,
        position.z
    );
}

VECTOR EnemyRootMotion::GetHipsWorldPosition(int modelHandle,VECTOR defaultPosition)
{
    if (modelHandle == -1)
        return defaultPosition;

    if (hipsFrameIndex == -1)
    {
        hipsFrameIndex = MV1SearchFrame( modelHandle,"mixamorig:Hips");

        if (hipsFrameIndex == -1)
            hipsFrameIndex = MV1SearchFrame(modelHandle,"Hips");
    }

    if (hipsFrameIndex == -1)
    {
        return defaultPosition;
    }

    MATRIX hipsMatrix =
        MV1GetFrameLocalWorldMatrix(modelHandle, hipsFrameIndex);

    return VGet(
        hipsMatrix.m[3][0],
        hipsMatrix.m[3][1],
        hipsMatrix.m[3][2]
    );
}

VECTOR EnemyRootMotion::GetHipsOffsetFromBase(
    int modelHandle,
    VECTOR basePosition
)
{
    VECTOR hipsPos = GetHipsWorldPosition(
        modelHandle,
        basePosition
    );

    VECTOR offset;

    offset.x = hipsPos.x - basePosition.x;
    offset.y = 0.0f;
    offset.z = hipsPos.z - basePosition.z;

    return offset;
}