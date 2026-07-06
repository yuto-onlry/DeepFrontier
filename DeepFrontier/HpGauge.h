#pragma once
#include "DxLib.h"

class HpGauge
{
private:
    int backHandle;
    int fillHandle;
    int frameHandle;

	float delayHpRate;  // 裏バーの割合を保持する変数
	int previousHp;     // 前回のHP値を保持する変数

    int delayWaitTimer;
    int delayWaitFrame;

    float delaySpeed;

public:
    HpGauge();
    ~HpGauge();
public:
    void Init(const char* backPath, const char* fillPath, const char* framePath);
    void Draw(int x, int y, int width, int height, int hp, int maxHp);
    void Release();

private:
    void Update(int hp, int maxHp);
};