#pragma once
#include "DxLib.h"
#include <cmath>
#include <csignal>

class InputManager
{
private:
    enum class PadButton
    {
        A = 3,
        B = 4,
        X = 1,
        Y = 2,
        LB = 5,
        RB = 6
    };

	// ジョイスティックの状態
    DINPUT_JOYSTATE currentInput;
    DINPUT_JOYSTATE prevInput;
	// ジョイスティックのハンドル
    float deadZone;
	// 接続判定
    bool isConnected;
public:
    InputManager();
    ~InputManager();

    void Init();
    void Update();

    VECTOR GetLeftStick() const;

    bool IsButton(PadButton button) const;
    bool IsButtonDown(PadButton button) const;
};