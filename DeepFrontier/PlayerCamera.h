#pragma once
#include "DxLib.h"

class PlayerCamera
{
private:
    VECTOR position;
    VECTOR target;

    float distance;       // プレイヤーから後ろへの距離
    float height;         // カメラの高さ
    float targetHeight;   // 注視点の高さ
    float followSpeed;    // 追従速度

    float angleY;     // 横回転
    float angleX;     // 縦回転
	float rotateSpeed;  // 回転速度

public:
    PlayerCamera();
    ~PlayerCamera();
public:
    //前方向ベクトルを取得
    VECTOR GetForward() const;
    //右方向ベクトルを取得
    VECTOR GetRight() const;
public:
    void Init();
    void Update(VECTOR playerPos, VECTOR rightStick);
    void Apply();
};