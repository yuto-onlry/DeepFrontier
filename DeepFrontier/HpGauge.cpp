#include "HpGauge.h"

HpGauge::HpGauge()
    : delayHpRate(1.0f),
    previousHp(-1),
    delayWaitTimer(0)
{
}

HpGauge::~HpGauge()
{
}

void HpGauge::Reset()
{
    delayHpRate = 1.0f;
    previousHp = -1;
    delayWaitTimer = 0;
}

void HpGauge::Update(int hp, int maxHp)
{
    if (maxHp <= 0)
        return;

    float hpRate = (float)hp / (float)maxHp;

    if (hpRate < 0.0f)
        hpRate = 0.0f;

    if (hpRate > 1.0f)
        hpRate = 1.0f;

    // 初回だけ現在HPに合わせる
    if (previousHp == -1)
    {
        previousHp = hp;
        delayHpRate = hpRate;
    }

    // HPが減った瞬間、裏バーの待機時間をセット
    if (hp < previousHp)
    {
        delayWaitTimer = 20;
    }
    
    if (hp > previousHp)
    {
        delayHpRate = hpRate;
        delayWaitTimer = 0;
    }

    previousHp = hp;

    // 裏バーを現在HPにゆっくり近づける
    if (delayHpRate > hpRate)
    {
        if (delayWaitTimer > 0)
        {
            delayWaitTimer--;
        }
        else
        {
            delayHpRate -= 0.002f;

            if (delayHpRate < hpRate)
            {
                delayHpRate = hpRate;
            }
        }
    }
    else
    {
        delayHpRate = hpRate;
    }
}

void HpGauge::Draw(int x, int y, int width, int height, int hp, int maxHp)
{
    if (maxHp <= 0)
        return;

    float hpRate = (float)hp / (float)maxHp;

    if (hpRate < 0.0f)
        hpRate = 0.0f;

    if (hpRate > 1.0f)
        hpRate = 1.0f;

    int hpWidth = (int)(width * hpRate);
    int delayHpWidth = (int)(width * delayHpRate);

    // 背景
    DrawBox(x, y, x + width, y + height, (30, 30, 40), TRUE);

    // 裏バー
    DrawBox(x, y, x + delayHpWidth, y + height, GetColor(255, 120, 0), TRUE);

    // 現在HPバー
    DrawBox(x,y,x + hpWidth,y + height,GetColor(60, 220, 80),TRUE);

    // 枠
    DrawBox(x, y, x + width, y + height, GetColor(255, 255, 255), TRUE);
}