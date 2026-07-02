#include "Weapon.h"
static VECTOR TransformPoint(const VECTOR& point, const MATRIX& matrix)
{
    VECTOR result;

    result.x =
        point.x * matrix.m[0][0] +
        point.y * matrix.m[1][0] +
        point.z * matrix.m[2][0] +
        matrix.m[3][0];

    result.y =
        point.x * matrix.m[0][1] +
        point.y * matrix.m[1][1] +
        point.z * matrix.m[2][1] +
        matrix.m[3][1];

    result.z =
        point.x * matrix.m[0][2] +
        point.y * matrix.m[1][2] +
        point.z * matrix.m[2][2] +
        matrix.m[3][2];

    return result;
}
Weapon::Weapon()
    : modelHandle(-1),
    ownerModelHandle(-1),
    attachFrameIndex(-1)
{
    offsetPosition = VGet(0.0f, 0.0f, 0.0f);
    offsetRotation = VGet(0.0f, 0.0f, 0.0f);
    scale = VGet(1.0f, 1.0f, 1.0f);

    attackLocalOffset = VGet(0.0f, 0.0f, 0.0f);
}

Weapon::~Weapon()
{
    Release();
}

bool Weapon::Init(int playerModelHandle, CharacterBase* owner)
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

    // 攻撃判定設定
    attackCollider.SetTag(ColliderTag::PlayerAttack);
    attackCollider.SetOwner(owner);
    attackCollider.SetRadius(35.0f);
    attackCollider.SetActive(false);

    // まずは武器の原点付近
    attackLocalOffset = VGet(0.0f, 0.0f, 0.0f);

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

    // 武器の位置に攻撃判定を追従
    VECTOR attackWorldPos = TransformPoint(attackLocalOffset, weaponMatrix);
    attackCollider.SetPosition(attackWorldPos);
}
void Weapon::Draw()
{
    if (modelHandle != -1)
    {
        MV1DrawModel(modelHandle);

        // 攻撃判定のデバッグ表示
        attackCollider.DrawDebug();

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
}void Weapon::Release()
{
    if (modelHandle != -1)
    {
        MV1DeleteModel(modelHandle);
        modelHandle = -1;
    }
}
// 攻撃判定の有効化・無効化
void Weapon::SetAttackColliderActive(bool active)
{
    attackCollider.SetActive(active);
}
// 攻撃判定のコライダーを取得
SphereCollider* Weapon::GetAttackCollider()
{
    return &attackCollider;
}

bool Weapon::IsValid() const
{
    return modelHandle != -1;
}
