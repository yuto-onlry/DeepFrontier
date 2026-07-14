#pragma once
#include "DxLib.h"

class EnemyRootMotion
{
private:
    int hipsFrameIndex;

    VECTOR startHipsOffset;

public:
    EnemyRootMotion();

    void Init();

    void Start(int modelHandle, VECTOR position);

    void AbsorbToPosition(int modelHandle,VECTOR& position);

    VECTOR GetBodyColliderPosition( int modelHandle,  VECTOR position, bool useHipsXZ);

private:
    VECTOR GetHipsWorldPosition(int modelHandle,VECTOR defaultPosition);

    VECTOR GetHipsOffsetFromBase(int modelHandle, VECTOR basePosition
    );
};