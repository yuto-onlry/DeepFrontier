#include "DxLib.h"
#include "CharacterManager.h"
#include "InputManager.h"
#include "UIManager.h"  
#include "ModelPreset.h"
#include "PlayerCamera.h"

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
    {
        InputManager inputManager;

        CharacterManager characterManager;
        characterManager.Init();

        UIManager uiManager;
        uiManager.Init();

        PlayerCamera playerCamera;
        playerCamera.Init();

        while (ProcessMessage() == 0)
        {
            if (CheckHitKey(KEY_INPUT_ESCAPE))
            {
                break;
            }

            ClearDrawScreen();

            inputManager.Update();
            VECTOR cameraForward = playerCamera.GetForward();
            VECTOR cameraRight = playerCamera.GetRight();

            characterManager.Update(inputManager, cameraForward, cameraRight);
            if (characterManager.GetPlayer() != nullptr)
            {
                VECTOR rightStick = inputManager.GetRightStick();

                playerCamera.Update(
                    characterManager.GetPlayer()->GetPosition(),
                    rightStick
                );
            }
            DrawGround();
            characterManager.Draw();

            if (characterManager.GetPlayer() != nullptr)
            {
                Player* player = characterManager.GetPlayer();

                uiManager.DrawPlayerHp(
                    player->GetHp(),
                    player->GetMaxHp()
                );
            }

            ScreenFlip();
            }
    }
	//モデルリソース解放
    ModelPreset::Release();

    DxLib_End();

    return 0;
}