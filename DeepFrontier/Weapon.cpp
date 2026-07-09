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
    WeaponModelHandle = MV1LoadModel("../3dModel/Player/Weapon/Weapon.mv1");
    if (WeaponModelHandle == -1)
        return false;

    // 右手フレームのインデックスを取得
    attachFrameIndex = MV1SearchFrame(ownerModelHandle, "mixamorig:RightHand");
    if (attachFrameIndex == -1)
        return false;

    // 武器の大きさ
    scale = VGet(0.01f, 0.01f, 0.01f);
    // 手元への調整
    offsetPosition = VGet(0.0f, 0.1f, -0.9f);
    // 武器の向き調整
    offsetRotation = VGet(DX_PI_F / 2.0f, 0.0f, DX_PI_F / 2.0f);
    // 攻撃判定設定
    for (int i = 0; i < AttackColliderCount; i++)
    {
        attackColliders[i].SetTag(ColliderTag::PlayerAttack);
        attackColliders[i].SetOwner(owner);
		// 攻撃判定の大きさ
        attackColliders[i].SetRadius(25.0f);
        attackColliders[i].SetActive(false);
    }

    // 今まで剣先に出ていた判定位置
    attackLocalOffset = VGet(0.0f, 0.0f, 0.0f);
    return true;
}
void Weapon::Update()
{
    if (WeaponModelHandle == -1 || ownerModelHandle == -1 || attachFrameIndex == -1)
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

    MV1SetMatrix(WeaponModelHandle, weaponMatrix);

    // 手元位置
    VECTOR rootPos = VGet(
        handMatrix.m[3][0],
        handMatrix.m[3][1],
        handMatrix.m[3][2]
    );

	// 剣先位置
    VECTOR tipPos = TransformPoint(attackLocalOffset, weaponMatrix);

	// 攻撃判定のコライダーを剣の根元から剣先まで均等に配置
    for (int i = 0; i < AttackColliderCount; i++)
    {
		// 攻撃判定のコライダーの位置を補間するための
        float startRate = 0.25f;
		// 剣先の位置を1.0fと過程
        float endRate = 1.0f;

        float t = startRate;

        if (AttackColliderCount > 1)
        {
            t = startRate + (endRate - startRate) * ((float)i / (float)(AttackColliderCount - 1));
        }

        VECTOR pos;
        pos.x = rootPos.x + (tipPos.x - rootPos.x) * t;
        pos.y = rootPos.y + (tipPos.y - rootPos.y) * t;
        pos.z = rootPos.z + (tipPos.z - rootPos.z) * t;

        attackColliders[i].SetPosition(pos);
    }
}
void Weapon::Draw()
{
    if (WeaponModelHandle != -1)
    {
        MV1DrawModel(WeaponModelHandle);

        // 攻撃判定のデバッグ表示
        //for (int i = 0; i < AttackColliderCount; i++)
        //{
        //    attackColliders[i].DrawDebug();
        //}     
    }
}void Weapon::Release()
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
