#include "UIManager.h"

UIManager::UIManager()
    : playerHpBackHandle(-1),
    playerHpFillHandle(-1),
    playerHpFrameHandle(-1)
{
}

UIManager::~UIManager()
{
    Release();
}

void UIManager::Init()
{
    //UIの読み込み
    playerHpBackHandle = LoadGraph("../UI/HP/PlayerHP/hp_back.png");
    playerHpFillHandle = LoadGraph("../UI/HP/PlayerHP/hp_fill.png");
    playerHpFrameHandle = LoadGraph("../UI/HP/PlayerHP/hp_frame.png");
}

void UIManager::DrawPlayerHp(int hp, int maxHp)
{
    if (maxHp <= 0)
        return;

    float hpRate = (float)hp / (float)maxHp;

    if (hpRate < 0.0f)
    {
        hpRate = 0.0f;
    }

    if (hpRate > 1.0f)
    {
        hpRate = 1.0f;
    }

    int x = 20;
    int y = 20;

    int width = 250;
    int height = 24;

    int hpWidth = (int)(width * hpRate);

    // HP裏
    if (playerHpBackHandle != -1)
    {
        DrawExtendGraph(x, y, x + width, y + height, playerHpBackHandle, TRUE);
    }

    // HP表
    if (playerHpFillHandle != -1)
    {
        DrawExtendGraph(x, y, x + hpWidth, y + height, playerHpFillHandle, TRUE);
    }

    // 枠
    if (playerHpFrameHandle != -1)
    {
        DrawExtendGraph(x, y, x + width, y + height, playerHpFrameHandle, TRUE);
    }

    DrawFormatString(x + width + 10, y + 2, GetColor(255, 255, 255), "%d / %d", hp, maxHp);
}




void UIManager::Release()
{
    if (playerHpBackHandle != -1)
    {
        DeleteGraph(playerHpBackHandle);
        playerHpBackHandle = -1;
    }

    if (playerHpFillHandle != -1)
    {
        DeleteGraph(playerHpFillHandle);
        playerHpFillHandle = -1;
    }

    if (playerHpFrameHandle != -1)
    {
        DeleteGraph(playerHpFrameHandle);
        playerHpFrameHandle = -1;
    }
}