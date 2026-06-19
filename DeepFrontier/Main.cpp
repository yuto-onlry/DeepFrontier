#include "DxLib.h"
#include "CharacterManager.h"
#include "InputManager.h"

//デバック用
void DrawGround()
{
    // 床
    DrawCube3D(
        VGet(-2000.0f, -40.0f, -2000.0f),
        VGet(2000.0f, -35.0f, 2000.0f),
        GetColor(30, 30, 40),
        GetColor(30, 30, 40),
        TRUE
    );

    // グリッド線
    for (int i = -2000; i <= 2000; i += 100)
    {
        DrawLine3D(
            VGet((float)i, -34.0f, -2000.0f),
            VGet((float)i, -34.0f, 2000.0f),
            GetColor(80, 80, 100)
        );

        DrawLine3D(
            VGet(-2000.0f, -34.0f, (float)i),
            VGet(2000.0f, -34.0f, (float)i),
            GetColor(80, 80, 100)
        );
    }

    // 中央のX軸
    DrawLine3D(
        VGet(-2000.0f, -33.0f, 0.0f),
        VGet(2000.0f, -33.0f, 0.0f),
        GetColor(120, 80, 80)
    );

    // 中央のZ軸
    DrawLine3D(
        VGet(0.0f, -33.0f, -2000.0f),
        VGet(0.0f, -33.0f, 2000.0f),
        GetColor(80, 120, 80)
    );
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    if (DxLib_Init() == -1)
    {
        return -1;
    }
    SetBackgroundColor(10, 10, 20);
    SetDrawScreen(DX_SCREEN_BACK);

    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);

    SetCameraPositionAndTarget_UpVecY(
        VGet(0.0f, 500.0f, -800.0f),
        VGet(0.0f, 0.0f, 0.0f)
    );
    InputManager inputManager;
    CharacterManager characterManager;
    characterManager.Init();

    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        ClearDrawScreen();
        inputManager.Update();
        characterManager.Update(inputManager);
        //デバック用
        DrawGround();
        characterManager.Draw();

        ScreenFlip();
    }


    characterManager.Release();

    DxLib_End();

    return 0;
}

