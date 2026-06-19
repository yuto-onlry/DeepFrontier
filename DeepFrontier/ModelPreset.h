#pragma once
#include "DxLib.h"

/// <summary>
/// モデルの読み込み、初期回転を設定するクラス
/// </summary>
class ModelPreset
{
public:
	// プレイヤーと敵のモデルを読み込む関数
	static int LoadPlayerModel();
	static int LoadLittleEnemyModel();
};