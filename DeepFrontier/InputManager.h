#pragma once
#include "DxLib.h"
#include <cmath>
#include <csignal>

class InputManager
{
public:
    enum class PadButton
    {
        A ,
        B ,
        X ,
        Y ,
        LB ,
        RB,
		Back,
		Start,
        LT,
		RT
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