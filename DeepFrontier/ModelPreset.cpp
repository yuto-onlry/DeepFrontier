#include "ModelPreset.h"

int ModelPreset::littleEnemyBaseHandle = -1;

int ModelPreset::LoadPlayerModel()
{
    int modelHandle = MV1LoadModel("../3dModel/Player/Player.mv1");

    if (modelHandle == -1)
    {
        return -1;
    }

    MV1SetScale(modelHandle, VGet(1.5f, 1.5f, 1.5f));
    MV1SetRotationXYZ(modelHandle, VGet(0.0f, 0.0f, 0.0f));

    return modelHandle;
}

int ModelPreset::LoadLittleEnemyModel()
{
	// 基本モデルがまだ読み込まれていない場合は読み込む
    if (littleEnemyBaseHandle == -1)
    {
        littleEnemyBaseHandle = MV1LoadModel("../3dModel/Enemy/LittleEnemy.mv1");

        if (littleEnemyBaseHandle == -1)
        {
            return -1;
        }

        MV1SetScale(littleEnemyBaseHandle, VGet(1.5f, 1.5f, 1.5f));
    }

    // 敵ごとに複製モデルを作る
    int modelHandle = MV1DuplicateModel(littleEnemyBaseHandle);

    if (modelHandle == -1)
    {
        return -1;
    }

    return modelHandle;
}

void ModelPreset::Release()
{
    if (littleEnemyBaseHandle != -1)
    {
        MV1DeleteModel(littleEnemyBaseHandle);
        littleEnemyBaseHandle = -1;
    }
}