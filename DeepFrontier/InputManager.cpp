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
	std::memset(&currentInput, 0, sizeof(DINPUT_JOYSTATE));
	std::memset(&prevInput, 0, sizeof(DINPUT_JOYSTATE));

	deadZone = 0.2f;
	isConnected = false;
}

void InputManager::Update()
{
    prevInput = currentInput;

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

bool InputManager::IsButton(int buttonNo) const
{
    if (buttonNo < 0 || buttonNo >= 32)
        return false;

    return currentInput.Buttons[buttonNo] != 0;
}

bool InputManager::IsButtonDown(int buttonNo) const
{
    if (buttonNo < 0 || buttonNo >= 32)
        return false;

    return currentInput.Buttons[buttonNo] != 0 &&
        prevInput.Buttons[buttonNo] == 0;
}