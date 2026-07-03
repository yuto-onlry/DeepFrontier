#include "LittleEnemy.h"
#include "ModelPreset.h"
#include "AnimationPreset.h"

LittleEnemy::LittleEnemy()
{
}

LittleEnemy::~LittleEnemy()
{
}

void LittleEnemy::Init()
{
    maxHp = 20;
    hp = maxHp;
    attack = 3;
    defense = 0;

    position = VGet(0.0f, 0.0f, 0.0f);
    velocity = VGet(0.0f, 0.0f, 0.0f);

    moveSpeed = 2.5f;
    attackRange = 70.0f;
    //モデルのセット
    modelHandle = ModelPreset::LoadLittleEnemyModel();

    if (modelHandle == -1)
        return;
    MV1SetPosition(modelHandle, position);

    // Enemyアニメーションセット
    AnimationPreset::SetAnimationLittleEnemy(animationManager, modelHandle);

    capsuleCollider.SetTag(ColliderTag::Enemy);
    capsuleCollider.SetOwner(this);
	// Enemyコライダーの大きさを設定
    capsuleCollider.SetRadius(35.0f);
	// Enemyコライダーの高さを設定
    capsuleCollider.SetHeight(360.0f);
    capsuleCollider.SetActive(true);
	// Enemyコライダーの位置を更新
    UpdateCollider();
    // Enemy攻撃判定の初期化
    InitAttackCollider();

    isDead = false;
}