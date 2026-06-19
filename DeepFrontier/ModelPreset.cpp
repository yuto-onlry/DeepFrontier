#include "ModelPreset.h"

int ModelPreset::LoadPlayerModel()
{
    int modelHandle = MV1LoadModel("../3dModel/Player/UAL2_Standard.mv1");
    if (modelHandle == -1)
        return -1;

    // モデルの大きさ
    MV1SetScale(modelHandle, VGet(200.0f, 200.0f, 200.0f));
    // モデルの横倒し補正
    MV1SetRotationXYZ(modelHandle,VGet(DX_PI_F / 2.0f, 0.0f, 0.0f));
    return modelHandle;
}

int ModelPreset::LoadLittleEnemyModel()
{
    int modelHandle = MV1LoadModel("../3dModel/Enemy/LittleEnemy.mv1");
    if (modelHandle == -1)
        return -1;
    MV1SetScale(modelHandle, VGet(1.5f, 1.5f, 1.5f));
    return modelHandle;
}