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
public:
    void Init();
    void DrawPlayerHp(int hp, int maxHp);
    void Release();
};