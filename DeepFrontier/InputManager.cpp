#include "InputManager.h"
#include <cstring>

InputManager::InputManager()
{
	Init();
}
InputManager::~InputManager()
{
}
void InputManager::Init()
{
	// ジョイスティックの状態を初期化
	std::memset(&currentInput, 0, sizeof(DINPUT_JOYSTATE));
	std::memset(&prevInput, 0, sizeof(DINPUT_JOYSTATE));

	deadZone = 0.2f;
	isConnected = false;
}

void InputManager::Update()
{
    prevInput = currentInput;
	// ジョイスティックの状態を取得
    if (GetJoypadDirectInputState(DX_INPUT_PAD1, &currentInput) == 0)
    {
        isConnected = true;
    }
    else
    {
        isConnected = false;
        std::memset(&currentInput, 0, sizeof(DINPUT_JOYSTATE));
    }
}
/// <summary>
/// 左スティックの入力値を取得する
/// </summary>
/// <returns></returns>
VECTOR InputManager::GetLeftStick() const
{
    float x = currentInput.X / 1000.0f;
    float y = currentInput.Y / 1000.0f;

    if (std::fabs(x) < deadZone)
        x = 0.0f;

    if (std::fabs(y) < deadZone)
        y = 0.0f;

    return VGet(x, 0.0f, -y);
}
/// <summary>
/// 右スティックの入力値を取得する
/// </summary>
/// <returns></returns>
VECTOR InputManager::GetRightStick() const
{
    float x = currentInput.Rx / 1000.0f;
    float y = currentInput.Ry / 1000.0f;

    if (std::fabs(x) < deadZone)
        x = 0.0f;

    if (std::fabs(y) < deadZone)
        y = 0.0f;

    return VGet(x, 0.0f, -y);
}   
/// <summary>
/// 指定したボタンが押されているかを判定する
/// </summary>
/// <param name="button"></param>
/// <returns></returns>
bool InputManager::IsButton(PadButton button) const
{
    int buttonNo = static_cast<int>(button);

    if (buttonNo < 0 || buttonNo >= 32)
    {
        return false;
    }

    return currentInput.Buttons[buttonNo] != 0;
}
/// <summary>
/// 指定したボタンが押された瞬間かを判定する
/// </summary>
/// <param name="button"></param>
/// <returns></returns>
bool InputManager::IsButtonDown(PadButton button) const
{
    int buttonNo = static_cast<int>(button);

    if (buttonNo < 0 || buttonNo >= 32) 
    {
        return false;
    }

    return currentInput.Buttons[buttonNo] != 0 &&
        prevInput.Buttons[buttonNo] == 0;
}