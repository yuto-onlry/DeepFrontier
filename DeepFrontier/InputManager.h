#pragma once
#include "DxLib.h"
#include <cmath>
#include <csignal>

class InputManager
{
private:
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

    bool IsButton(int buttonNo) const;
    bool IsButtonDown(int buttonNo) const;
};