#pragma once

#include "DxLib.h"

class PlayerRootMotion
{
private:
    int hipsFrameIndex;

    bool isActive;

    VECTOR attackRootStartPosition;
    VECTOR attackRootStartHipsOffset;
    VECTOR attackRootPrevHipsOffset;

    VECTOR attackRootMoveDir;
    bool hasAttackRootMoveDir;

public:
    PlayerRootMotion();
public:
    void Init(int modelHandle);

    void Start(int modelHandle, VECTOR position);
    void Update(int modelHandle, VECTOR& position);
    void End(int modelHandle, VECTOR& position);

    bool IsActive() const;

    VECTOR GetHipsWorldPosition(int modelHandle, VECTOR defaultPosition) const;
    void PositionCorrection(VECTOR correction);

private:
    VECTOR GetHipsOffsetFromBase(int modelHandle, VECTOR basePosition) const;
};