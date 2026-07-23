#include "TitleManager.h"
#include <cstring>

TitleManager::TitleManager()
    : selectIndex(0),
    startRequest(false),
    endRequest(false),
    moveCoolTime(0)
{
}

TitleManager::~TitleManager()
{
    Release();
}

void TitleManager::Init()
{
    selectIndex = 0;

    startRequest = false;
    endRequest = false;

    moveCoolTime = 0;
}

void TitleManager::Update(const InputManager& inputManager)
{
    if (moveCoolTime > 0)
    {
        moveCoolTime--;
    }

    VECTOR leftStick = inputManager.GetLeftStick();

    if (moveCoolTime <= 0)
    {
        // 上下選択
        if (leftStick.z > 0.5f)
        {
            selectIndex--;

            if (selectIndex < 0)
            {
                selectIndex = (int)MenuItem::Max - 1;
            }

            moveCoolTime = 15;
        }
        else if (leftStick.z < -0.5f)
        {
            selectIndex++;

            if (selectIndex >= (int)MenuItem::Max)
            {
                selectIndex = 0;
            }

            moveCoolTime = 15;
        }
    }

    // Aボタンで決定
    if (inputManager.IsButtonDown(InputManager::PadButton::A))
    {
        if (selectIndex == (int)MenuItem::Start)
        {
            startRequest = true;
        }
        else if (selectIndex == (int)MenuItem::End)
        {
            endRequest = true;
        }
    }
}

void TitleManager::Draw()
{
    int screenW = 0;
    int screenH = 0;

    GetDrawScreenSize(&screenW, &screenH);

    DrawBox(
        0,
        0,
        screenW,
        screenH,
        GetColor(5, 5, 15),
        TRUE
    );

    const char* titleText = "DEEP FRONTIER";
    const char* startText = "START";
    const char* endText = "END DEBUG";

    int titleW = GetDrawStringWidth(titleText, (int)strlen(titleText));
    int startW = GetDrawStringWidth(startText, (int)strlen(startText));
    int endW = GetDrawStringWidth(endText, (int)strlen(endText));

    DrawString(
        screenW / 2 - titleW / 2,
        screenH / 2 - 150,
        titleText,
        GetColor(255, 255, 255)
    );

    int startColor = GetColor(180, 180, 180);
    int endColor = GetColor(180, 180, 180);

    if (selectIndex == (int)MenuItem::Start)
    {
        startColor = GetColor(180, 255, 180);
    }
    else if (selectIndex == (int)MenuItem::End)
    {
        endColor = GetColor(180, 255, 180);
    }

    int startY = screenH / 2;
    int endY = screenH / 2 + 70;

    DrawString(
        screenW / 2 - startW / 2,
        startY,
        startText,
        startColor
    );

    DrawString(
        screenW / 2 - endW / 2,
        endY,
        endText,
        endColor
    );

    // 選択カーソル
    int cursorY = startY;

    if (selectIndex == (int)MenuItem::End)
    {
        cursorY = endY;
    }

    DrawString(
        screenW / 2 - 140,
        cursorY,
        ">",
        GetColor(255, 255, 255)
    );

    DrawString(
        screenW / 2 - 150,
        screenH / 2 + 150,
        "LEFT STICK UP/DOWN : SELECT",
        GetColor(120, 120, 120)
    );

    DrawString(
        screenW / 2 - 90,
        screenH / 2 + 190,
        "A BUTTON : DECIDE",
        GetColor(120, 120, 120)
    );
}

void TitleManager::Release()
{
}

bool TitleManager::IsStartRequest() const
{
    return startRequest;
}

bool TitleManager::IsEndRequest() const
{
    return endRequest;
}