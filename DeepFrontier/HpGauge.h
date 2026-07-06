#pragma once
#include "DxLib.h"

class HpGauge
{
private:
    float delayHpRate;
    int previousHp;
    int delayWaitTimer;

public:
    HpGauge();
    ~HpGauge();

    void Update(int hp, int maxHp);
    void Draw(int x, int y, int width, int height, int hp, int maxHp);

    void Reset();
};