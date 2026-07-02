#include "Weapon.h"

Weapon::Weapon()
    : modelHandle(-1),
    ownerModelHandle(-1),
    attachFrameIndex(-1)
{
    offsetPosition = VGet(0.0f, 0.0f, 0.0f);
    offsetRotation = VGet(0.0f, 0.0f, 0.0f);
    scale = VGet(1.0f, 1.0f, 1.0f);
}

Weapon::~Weapon()
{
    Release();
}

bool Weapon::Init(int playerModelHandle)
{
    ownerModelHandle = playerModelHandle;
    // 武器モデル読み込み
    modelHandle = MV1LoadModel("../3dModel/Player/Weapon/Weapon.mv1");
    if (modelHandle == -1)
        return false;
	// 右手フレームのインデックスを取得
    attachFrameIndex = MV1SearchFrame(ownerModelHandle, "hand_r");

    if (attachFrameIndex == -1)
        attachFrameIndex = MV1SearchFrame(ownerModelHandle, "mixamorig:RightHand");

    if (attachFrameIndex == -1)
        attachFrameIndex = MV1SearchFrame(ownerModelHandle, "RightHand");

    if (attachFrameIndex == -1)       
        return false;

    // 武器の大きさ
    scale = VGet(0.01f, 0.01f, 0.01f);
    // 手元への調整
    offsetPosition = VGet(0.0f, 0.1f, -0.9f);
    // 武器の向き調整
    offsetRotation = VGet(DX_PI_F / 2.0f, 0.0f, DX_PI_F / 2.0f);
    return true;
}

void Weapon::Update()
{
    if (modelHandle == -1 || ownerModelHandle == -1 || attachFrameIndex == -1)
    {
        return;
    }

    MATRIX handMatrix = MV1GetFrameLocalWorldMatrix(ownerModelHandle, attachFrameIndex);

    MATRIX scaleMatrix = MGetScale(scale);

    MATRIX rotX = MGetRotX(offsetRotation.x);
    MATRIX rotY = MGetRotY(offsetRotation.y);
    MATRIX rotZ = MGetRotZ(offsetRotation.z);

    MATRIX rotMatrix = MMult(MMult(rotX, rotY), rotZ);

    MATRIX transMatrix = MGetTranslate(offsetPosition);

    MATRIX offsetMatrix = MMult(MMult(scaleMatrix, rotMatrix), transMatrix);

    MATRIX weaponMatrix = MMult(offsetMatrix, handMatrix);

    MV1SetMatrix(modelHandle, weaponMatrix);
}   
void Weapon::Draw()
{
    if (modelHandle != -1)
    {
        MV1DrawModel(modelHandle);

        DrawString(
            20,
            100,
            "Weapon Draw OK",
            GetColor(255, 255, 0)
        );
    }
    else
    {
        DrawString(
            20,
            100,
            "Weapon modelHandle == -1",
            GetColor(255, 0, 0)
        );
    }
}
void Weapon::Release()
{
    if (modelHandle != -1)
    {
        MV1DeleteModel(modelHandle);
        modelHandle = -1;
    }
}

bool Weapon::IsValid() const
{
    return modelHandle != -1;
}
