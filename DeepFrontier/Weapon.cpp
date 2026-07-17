#include "Weapon.h"

//ローカル座標からワールド座標に変換
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
    : WeaponModelHandle(-1),
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
    WeaponModelHandle = MV1LoadModel("../3dModel/Weapon/Weapon.mv1");
    if (WeaponModelHandle == -1)
    {
        return false;
    }

    // 右手フレームのインデックスを取得
    attachFrameIndex = MV1SearchFrame(ownerModelHandle, "mixamorig:RightHand");
    if (attachFrameIndex == -1)
    {
        return false;
    }

    // 武器の見た目調整
    scale = VGet(1.2f, 1.2f, 1.2f);
    offsetRotation = VGet(0.0f, 0.0f, DX_PI_F / 2.0f);
    offsetPosition = VGet(0.0f, 0.0f, 0.0f);

    // 攻撃判定設定
    for (int i = 0; i < AttackColliderCount; i++)
    {
        attackColliders[i].SetTag(ColliderTag::PlayerAttack);
        attackColliders[i].SetOwner(owner);
        attackColliders[i].SetRadius(15.0f);
        attackColliders[i].SetActive(false);
    }

    return true;
}
void Weapon::Update()
{
    if (WeaponModelHandle == -1 || ownerModelHandle == -1 || attachFrameIndex == -1)
    {
        return;
    }

    // 右手のワールド行列
    MATRIX handMatrix = MV1GetFrameLocalWorldMatrix(ownerModelHandle, attachFrameIndex);

    // 武器の調整行列
    MATRIX scaleMatrix = MGetScale(scale);

    MATRIX rotX = MGetRotX(offsetRotation.x);
    MATRIX rotY = MGetRotY(offsetRotation.y);
    MATRIX rotZ = MGetRotZ(offsetRotation.z);

    MATRIX rotMatrix = MMult(MMult(rotX, rotY), rotZ);

    MATRIX transMatrix = MGetTranslate(offsetPosition);

    MATRIX offsetMatrix = MMult(
        MMult(scaleMatrix, rotMatrix),
        transMatrix
    );

    // 武器を右手に追従させる
    MATRIX weaponMatrix = MMult(offsetMatrix, handMatrix);
    MV1SetMatrix(WeaponModelHandle, weaponMatrix);

    // 刃の当たり判定位置
    VECTOR bladeStartLocalOffset = VGet(0.0f, -22.0f, 0.0f);
    VECTOR bladeTipLocalOffset = VGet(0.0f, -70.0f, 0.0f);

    VECTOR bladeStartPos = TransformPoint(bladeStartLocalOffset, weaponMatrix);
    VECTOR bladeTipPos = TransformPoint(bladeTipLocalOffset, weaponMatrix);

    // 刃の根元から剣先まで球を並べる
    for (int i = 0; i < AttackColliderCount; i++)
    {
        float t = 0.0f;

        if (AttackColliderCount > 1)
        {
            t = (float)i / (float)(AttackColliderCount - 1);
        }

        VECTOR pos;
        pos.x = bladeStartPos.x + (bladeTipPos.x - bladeStartPos.x) * t;
        pos.y = bladeStartPos.y + (bladeTipPos.y - bladeStartPos.y) * t;
        pos.z = bladeStartPos.z + (bladeTipPos.z - bladeStartPos.z) * t;

        attackColliders[i].SetPosition(pos);
    }
}
void Weapon::Draw()
{
    if (WeaponModelHandle == -1)
        return;

    MV1DrawModel(WeaponModelHandle);

    // 攻撃判定のデバッグ表示
    for (int i = 0; i < AttackColliderCount; i++)
        attackColliders[i].DrawDebug();
    
}
void Weapon::Release()
{
    if (WeaponModelHandle != -1)
    {
        MV1DeleteModel(WeaponModelHandle);
        WeaponModelHandle = -1;
    }
}
// 攻撃判定の有効化・無効化
void Weapon::SetAttackColliderActive(bool active)
{
    for (int i = 0; i < AttackColliderCount; i++)
    {
        attackColliders[i].SetActive(active);
    }
}
// 攻撃判定のコライダー数を取得
int Weapon::GetAttackColliderCount() const
{
    return AttackColliderCount;
}

// 攻撃判定のコライダーを取得
SphereCollider* Weapon::GetAttackCollider(int index)
{
    if (index < 0 || index >= AttackColliderCount)
    {
        return nullptr;
    }

    return &attackColliders[index];
}
bool Weapon::IsValid() const
{
    return WeaponModelHandle != -1;
}
