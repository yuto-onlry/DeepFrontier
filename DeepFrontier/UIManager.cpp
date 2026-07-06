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
}

void UIManager::DrawPlayerHp(int hp, int maxHp)
{
    playerHpGauge.Update(hp, maxHp);

    int screenWidth = 0;
    int screenHeight = 0;
    GetScreenState(&screenWidth, &screenHeight, nullptr);

    int x = 20;
    int y = screenHeight - 60;

    playerHpGauge.Draw(x, y, 250, 24, hp, maxHp);
	// HP数値を描画
    DrawFormatString(x + 260, y + 2, GetColor(255, 255, 255), "%d / %d", hp, maxHp);
}

void UIManager::Release()
{
}