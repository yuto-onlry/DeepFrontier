#include "DxLib.h"
#include "Player.h"
#include "EnemyBase.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    if (DxLib_Init() == -1)
    {
        return -1;
    }

    SetDrawScreen(DX_SCREEN_BACK);

    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);

    SetCameraPositionAndTarget_UpVecY(
        VGet(0.0f, 500.0f, -800.0f),
        VGet(0.0f, 0.0f, 0.0f)
    );

    Player player;
    EnemyBase enemy;

    player.Init();
    enemy.Init();

    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        ClearDrawScreen();

        player.Update();
        enemy.Update(player.GetPosition());

        player.Draw();
        enemy.Draw();

        ScreenFlip();
    }

    DxLib_End();

    return 0;
}