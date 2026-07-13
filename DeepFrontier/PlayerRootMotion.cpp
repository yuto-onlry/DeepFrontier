#include "PlayerRootMotion.h"
#include <cmath>

PlayerRootMotion::PlayerRootMotion()
    : hipsFrameIndex(-1),
    isActive(false),
    attackRootStartPosition(VGet(0.0f, 0.0f, 0.0f)),
    attackRootStartHipsOffset(VGet(0.0f, 0.0f, 0.0f)),
    attackRootPrevHipsOffset(VGet(0.0f, 0.0f, 0.0f)),
    attackRootMoveDir(VGet(0.0f, 0.0f, 1.0f)),
    hasAttackRootMoveDir(false)
{
}

void PlayerRootMotion::Init(int modelHandle)
{
    hipsFrameIndex = MV1SearchFrame(modelHandle, "mixamorig:Hips");
}

void PlayerRootMotion::Start(int modelHandle, VECTOR position)
{
    if (modelHandle == -1 || hipsFrameIndex == -1)
    {
        return;
    }

    isActive = true;

    attackRootStartPosition = position;

    hasAttackRootMoveDir = false;
    attackRootMoveDir = VGet(0.0f, 0.0f, 1.0f);

    MV1SetPosition(modelHandle, attackRootStartPosition);

    attackRootStartHipsOffset =
        GetHipsOffsetFromBase(modelHandle, attackRootStartPosition);

    attackRootPrevHipsOffset = attackRootStartHipsOffset;
}

void PlayerRootMotion::Update(int modelHandle, VECTOR& position)
{
    if (isActive == false)
    {
        return;
    }

    if (modelHandle == -1 || hipsFrameIndex == -1)
    {
        return;
    }

    MV1SetPosition(modelHandle, attackRootStartPosition);

    VECTOR currentHipsOffset =
        GetHipsOffsetFromBase(modelHandle, attackRootStartPosition);

    VECTOR delta = VSub(currentHipsOffset, attackRootPrevHipsOffset);
    delta.y = 0.0f;

    float deltaLength = sqrtf(delta.x * delta.x + delta.z * delta.z);

    if (hasAttackRootMoveDir == false && deltaLength > 0.5f)
    {
        attackRootMoveDir.x = delta.x / deltaLength;
        attackRootMoveDir.y = 0.0f;
        attackRootMoveDir.z = delta.z / deltaLength;

        hasAttackRootMoveDir = true;
    }

    if (hasAttackRootMoveDir == true)
    {
        float moveDistance =
            delta.x * attackRootMoveDir.x +
            delta.z * attackRootMoveDir.z;

        if (moveDistance < 0.0f)
        {
            moveDistance = 0.0f;
        }

        const float maxMovePerFrame = 5.0f;

        if (moveDistance > maxMovePerFrame)
        {
            moveDistance = maxMovePerFrame;
        }

        const float rootMotionRate = 0.35f;

        position.x += attackRootMoveDir.x * moveDistance * rootMotionRate;
        position.z += attackRootMoveDir.z * moveDistance * rootMotionRate;
    }

    attackRootPrevHipsOffset = currentHipsOffset;

    MV1SetPosition(modelHandle, position);
}

void PlayerRootMotion::End(int modelHandle, VECTOR& position)
{
    if (modelHandle != -1 && hipsFrameIndex != -1)
    {
        VECTOR finalHipsPos = GetHipsWorldPosition(modelHandle, position);

        position.x = finalHipsPos.x - attackRootStartHipsOffset.x;
        position.z = finalHipsPos.z - attackRootStartHipsOffset.z;
    }

    isActive = false;
    hasAttackRootMoveDir = false;

    MV1SetPosition(modelHandle, position);
}

bool PlayerRootMotion::IsActive() const
{
    return isActive;
}

VECTOR PlayerRootMotion::GetHipsWorldPosition(
    int modelHandle,
    VECTOR defaultPosition
) const
{
    if (modelHandle == -1 || hipsFrameIndex == -1)
    {
        return defaultPosition;
    }

    MATRIX hipsMatrix = MV1GetFrameLocalWorldMatrix(modelHandle, hipsFrameIndex);

    return VGet(
        hipsMatrix.m[3][0],
        hipsMatrix.m[3][1],
        hipsMatrix.m[3][2]
    );
}

VECTOR PlayerRootMotion::GetHipsOffsetFromBase(
    int modelHandle,
    VECTOR basePosition
) const
{
    if (modelHandle == -1 || hipsFrameIndex == -1)
    {
        return VGet(0.0f, 0.0f, 0.0f);
    }

    VECTOR hipsPos = GetHipsWorldPosition(modelHandle, basePosition);

    VECTOR offset;

    offset.x = hipsPos.x - basePosition.x;
    offset.y = 0.0f;
    offset.z = hipsPos.z - basePosition.z;

    return offset;
}