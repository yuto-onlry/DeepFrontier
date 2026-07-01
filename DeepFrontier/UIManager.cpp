#include "UIManager.h"

UIManager::UIManager()
    : playerHpBackHandle(-1),
    playerHpFillHandle(-1),
    playerHpFrameHandle(-1),
	playerDelayHpRate(1.0f)
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
        hpRate = 0.0f;
    if (hpRate > 1.0f)
        hpRate = 1.0f;

    // 裏バーを現在HPにゆっくり近づける
    if (playerDelayHpRate > hpRate)
    {
        //減少速度
        playerDelayHpRate -= 0.005f;

        if (playerDelayHpRate < hpRate)
        {
            playerDelayHpRate = hpRate;
        }
    }
    else
    {
		//HPが裏バーより増えた場合のリセット
        playerDelayHpRate = hpRate;
    }

    int x = 20;
    int y = 20;

    int width = 250;
    int height = 24;
	// 現在HPバーの幅計算
    int hpWidth = (int)(width * hpRate);
	// 裏バーの幅計算
    int delayHpWidth = (int)(width * playerDelayHpRate);

    //背景
    DrawBox(x, y, x + width, y + height, GetColor(30, 30, 40), TRUE);

    //裏バー
    if (playerHpBackHandle != -1)
    {
        DrawExtendGraph(x, y, x + delayHpWidth, y + height, playerHpBackHandle, TRUE);
    }

    //現在HPバー
    if (playerHpFillHandle != -1)
    {
        DrawExtendGraph(x, y, x + hpWidth, y + height, playerHpFillHandle, TRUE);
    }

    // 枠
    if (playerHpFrameHandle != -1)
    {
        DrawExtendGraph(x, y, x + width, y + height, playerHpFrameHandle, TRUE);
    }
	// HP数値デバック表示
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