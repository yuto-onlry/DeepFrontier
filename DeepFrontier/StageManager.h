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

    // 地面の高さ
    float GetGroundY() const;

    //デバック用
    void DebugGroundRay(VECTOR targetPosition) const;
};