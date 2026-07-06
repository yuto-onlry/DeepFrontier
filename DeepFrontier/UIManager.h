#pragma once
#include "DxLib.h"
#include "HpGauge.h"

class UIManager
{
private:
    HpGauge playerHpGauge;

public:
    UIManager();
    ~UIManager();

    void Init();
    void DrawPlayerHp(int hp, int maxHp);
    void Release();
};