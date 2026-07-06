#include "HpGauge.h"

HpGauge::HpGauge()
    : backHandle(-1),
    fillHandle(-1),
    frameHandle(-1),
    delayHpRate(1.0f),
    previousHp(-1),
    delayWaitTimer(0),
    delayWaitFrame(20),
    delaySpeed(0.002f)
{
}

HpGauge::~HpGauge()
{
    Release();
}

void HpGauge::Init(
    const char* backPath,
    const char* fillPath,
    const char* framePath
)
{
    backHandle = LoadGraph(backPath);
    fillHandle = LoadGraph(fillPath);
    frameHandle = LoadGraph(framePath);
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
        delayWaitTimer = delayWaitFrame;
    }

    // HPが回復した場合は、裏バーもすぐ合わせる
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
            delayHpRate -= delaySpeed;

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

void HpGauge::Draw(
    int x,
    int y,
    int width,
    int height,
    int hp,
    int maxHp
)
{
    if (maxHp <= 0)
        return;

    Update(hp, maxHp);

    float hpRate = (float)hp / (float)maxHp;

    if (hpRate < 0.0f)
        hpRate = 0.0f;

    if (hpRate > 1.0f)
        hpRate = 1.0f;

    int hpWidth = (int)(width * hpRate);
    int delayHpWidth = (int)(width * delayHpRate);

    // 背景
    DrawBox(
        x,
        y,
        x + width,
        y + height,
        GetColor(30, 30, 40),
        TRUE
    );

    // 裏バー
    if (backHandle != -1)
    {
        DrawExtendGraph(
            x,
            y,
            x + delayHpWidth,
            y + height,
            backHandle,
            TRUE
        );
    }

    // 現在HPバー
    if (fillHandle != -1)
    {
        DrawExtendGraph(
            x,
            y,
            x + hpWidth,
            y + height,
            fillHandle,
            TRUE
        );
    }

    // 枠
    if (frameHandle != -1)
    {
        DrawExtendGraph(
            x,
            y,
            x + width,
            y + height,
            frameHandle,
            TRUE
        );
    }
}

void HpGauge::Release()
{
    if (backHandle != -1)
    {
        DeleteGraph(backHandle);
        backHandle = -1;
    }

    if (fillHandle != -1)
    {
        DeleteGraph(fillHandle);
        fillHandle = -1;
    }

    if (frameHandle != -1)
    {
        DeleteGraph(frameHandle);
        frameHandle = -1;
    }
}