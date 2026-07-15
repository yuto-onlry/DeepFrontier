#pragma once
#include "DxLib.h"

class StageManager
{
private:
    int stageModelHandle;

    VECTOR position;
    VECTOR scale;
    VECTOR rotation;

    // ステージ判定用
    VECTOR stageCenter;
    float stageRadius;
    float groundY;

public:
    StageManager();
    ~StageManager();

public:
    bool Init();
    void Update();
    void Draw();
    void Release();

    // ステージ内に収める
    VECTOR ClampPosition(VECTOR targetPosition, float radius) const;
    VECTOR ClampCameraPosition(VECTOR cameraPosition) const;

    // 地面の高さ
    float GetGroundY() const;
};