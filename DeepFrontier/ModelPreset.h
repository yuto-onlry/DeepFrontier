#pragma once
#include "DxLib.h"

/// <summary>
/// モデルの読み込み、初期回転を設定するクラス
/// </summary>
class ModelPreset
{
private:
    static int littleEnemyBaseHandle;

public:
    static int LoadPlayerModel();
    static int LoadLittleEnemyModel();

    static void Release();
};