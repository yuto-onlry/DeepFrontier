#include "DxLib.h"
#include "GameManager.h"
#include "ModelPreset.h"

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
        GameManager gameManager;
        gameManager.Init();

        while (ProcessMessage() == 0)
        {
            if (CheckHitKey(KEY_INPUT_ESCAPE))
            {
                break;
            }

            ClearDrawScreen();

            gameManager.Update();
            gameManager.Draw();

            ScreenFlip();
        }
    }

    // 共有モデルリソース解放
    ModelPreset::Release();

    DxLib_End();

    return 0;
}