#include "UIManager.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
    Release();
}

void UIManager::Init()
{
    playerHpGauge.Init(
        "../UI/HP/PlayerHP/hp_back.png",
        "../UI/HP/PlayerHP/hp_fill.png",
        "../UI/HP/PlayerHP/hp_frame.png"
    );
}

void UIManager::DrawPlayerHp(int hp, int maxHp)
{
    int screenWidth = 0;
    int screenHeight = 0;
    GetScreenState(&screenWidth, &screenHeight, nullptr);

    int x = 20;
    int y = screenHeight - 60;

    int width = 250;
    int height = 24;

    playerHpGauge.Draw(
        x,
        y,
        width,
        height,
        hp,
        maxHp
    );

    // HP数値デバッグ表示
    DrawFormatString(
        x + width + 10,
        y + 2,
        GetColor(255, 255, 255),
        "%d / %d",
        hp,
        maxHp
    );
}

void UIManager::Release()
{
    playerHpGauge.Release();
}