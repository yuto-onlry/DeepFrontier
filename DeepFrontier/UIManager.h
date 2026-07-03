#pragma once
#include "DxLib.h"

class UIManager
{
private:
    int playerHpBackHandle;
    int playerHpFillHandle;
    int playerHpFrameHandle;

    float playerDelayHpRate;
    int previousPlayerHp;
    int playerDelayWaitTimer;


public:
    UIManager();
    ~UIManager();
public:
    void Init();
    void DrawPlayerHp(int hp, int maxHp);
    void Release();
};