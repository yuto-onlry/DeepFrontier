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
        groundY(0.0f),
        debugHasGroundRay(false),
        debugGroundRayHit(false),
        debugGroundRayStart(VGet(0.0f, 0.0f, 0.0f)),
        debugGroundRayEnd(VGet(0.0f, 0.0f, 0.0f)),
        debugGroundHitPosition(VGet(0.0f, 0.0f, 0.0f))
    {
    }

    StageManager::~StageManager()
    {
        Release();
    }

    bool StageManager::Init()
    {
        stageModelHandle = MV1LoadModel("../3dModel/Stage/Stage00.mv1");

        if (stageModelHandle == -1)
        {
            return false;
        }

        position = VGet(0.0f, 0.0f, 0.0f);
        scale = VGet(7.0f, 7.0f, 7.0f);
        rotation = VGet(0.0f, 0.0f, 0.0f);

        // ステージ判定用
        stageCenter = VGet(0.0f, 0.0f, 0.0f);
        stageRadius =2200.0f;
        groundY = 0.0f;
        //壁の押し出し判定
        circleWall.clear();


        MV1SetPosition(stageModelHandle, position);
        MV1SetScale(stageModelHandle, scale);
        MV1SetRotationXYZ(stageModelHandle, rotation);

        // ステージの当たり判定情報を作成
        MV1SetupCollInfo(stageModelHandle, -1, 16, 4, 16);
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
        if (debugHasGroundRay == true)
        {
            // デバッグ線をモデルに隠されないようにする
            SetUseZBuffer3D(FALSE);
            SetWriteZBuffer3D(FALSE);

            // レイ全体：水色
            DrawLine3D(
                debugGroundRayStart,
                debugGroundRayEnd,
                GetColor(0, 255, 255)
            );

            if (debugGroundRayHit == true)
            {
                // ヒット位置：赤い球
                DrawSphere3D(
                    debugGroundHitPosition,
                    30.0f,
                    16,
                    GetColor(255, 0, 0),
                    GetColor(255, 0, 0),
                    TRUE
                );

                // 開始位置からヒット位置まで：赤線
                DrawLine3D(
                    debugGroundRayStart,
                    debugGroundHitPosition,
                    GetColor(255, 0, 0)
                );
            }

            SetUseZBuffer3D(TRUE);
            SetWriteZBuffer3D(TRUE);
        }
        DrawWallDebug();
    }

    void StageManager::Release()
    {

        if (stageModelHandle != -1)
        {

             MV1TerminateCollInfo(stageModelHandle, -1);


            MV1DeleteModel(stageModelHandle);

            stageModelHandle = -1;
        }

    }
    VECTOR StageManager::ClampPosition(VECTOR targetPosition, float radius) const
    {
        VECTOR result = targetPosition;
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

        VECTOR start = VGet(targetPosition.x,targetPosition.y + 1000.0f,targetPosition.z);

        VECTOR end = VGet(targetPosition.x,targetPosition.y - 1000.0f,targetPosition.z);

        MV1_COLL_RESULT_POLY hitResult = MV1CollCheck_Line(stageModelHandle, -1, start, end);

        if (hitResult.HitFlag == TRUE)
        {
            outGroundY = hitResult.HitPosition.y;
            return true;
        }

        return false;
    }

    VECTOR StageManager::FitPositionToGround(VECTOR targetPosition,float footOffset) const
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

    VECTOR StageManager::ResolveWallCollision(VECTOR targetPosition, float characterRadius) const
    {
        VECTOR result = targetPosition;

        for (const CircleWall& wall : circleWall) {
            float dx = result.x - wall.center.x;
            float dz = result.z - wall.center.z;
            float distanceSq = dx * dx + dz * dz;

            float limitDistance = wall.radius + characterRadius;
            float limitDistanceSq = limitDistance * limitDistance;

            if (distanceSq < limitDistanceSq) {
                float distance = sqrtf(distanceSq);
                if (distance > 0.001f) {
                    dx /= distance;
                    dz /= distance;

                    result.x = wall.center.x + dx * limitDistance;
                    result.z = wall.center.z + dz * limitDistance;
                }
                else {
                    result.x = wall.center.x + limitDistance;
                    result.z = wall.center.z;
                }
            }
        }
        return result;
    }

    float StageManager::GetGroundY() const
    {
        return groundY;
    }
    void StageManager::DebugGroundRay(VECTOR targetPosition) const
    {
        if (stageModelHandle == -1)
        {
            debugHasGroundRay = false;
            return;
        }

        debugHasGroundRay = true;
        debugGroundRayHit = false;

        // プレイヤーの足元少し上から開始
        debugGroundRayStart = VGet(
            targetPosition.x,
            targetPosition.y + 10.0f,
            targetPosition.z
        );

        // 足元から下方向だけを見る
        debugGroundRayEnd = VGet(
            targetPosition.x,
            targetPosition.y - 300.0f,
            targetPosition.z
        );

        MV1_COLL_RESULT_POLY hitResult = MV1CollCheck_Line(
            stageModelHandle,
            -1,
            debugGroundRayStart,
            debugGroundRayEnd
        );

        if (hitResult.HitFlag == TRUE)
        {
            debugGroundRayHit = true;
            debugGroundHitPosition = hitResult.HitPosition;
        }
    }

    void StageManager::DrawWallDebug() const
    {
        for (const CircleWall& wall : circleWall)
        {
            DrawCircleXZ(
                VGet(wall.center.x, groundY + 20.0f, wall.center.z),
                wall.radius,
                32,
                GetColor(255, 0, 255)
            );
        }
    }