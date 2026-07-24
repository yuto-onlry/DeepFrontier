#pragma once
#include "DxLib.h"
#include <vector>

class StageManager
{
private:
    struct CircleWall
    {
        VECTOR center;
        float radius;
    };
private:
    int stageModelHandle;
    std::vector<CircleWall> circleWall;

    VECTOR position;
    VECTOR scale;
    VECTOR rotation;

    // ステージ判定用
    VECTOR stageCenter;
    float stageRadius;
    float groundY;
    
//デバック用
private:
    mutable bool debugHasGroundRay;
    mutable bool debugGroundRayHit;

    mutable VECTOR debugGroundRayStart;
    mutable VECTOR debugGroundRayEnd;
    mutable VECTOR debugGroundHitPosition;
    //=======================================
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
public:
    bool GetGroundYByRay(VECTOR targetPosition, float& outGroundY) const;
    VECTOR FitPositionToGround(VECTOR targetPosition, float footOffset) const;
    VECTOR ResolveWallCollision(VECTOR targetPosition, float characterRadius) const;
    // 地面の高さ
    float GetGroundY() const;

    //デバック用
    void DebugGroundRay(VECTOR targetPosition) const;
    void DrawWallDebug() const;
};