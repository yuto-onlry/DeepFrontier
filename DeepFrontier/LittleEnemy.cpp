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
    hp = 20;
    attack = 3;
    defense = 0;

    position = VGet(0.0f, 0.0f, 0.0f);
    velocity = VGet(0.0f, 0.0f, 0.0f);

    moveSpeed = 1.5f;
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
    capsuleCollider.SetRadius(35.0f);
    capsuleCollider.SetHeight(360.0f);
    capsuleCollider.SetActive(true);

    UpdateCollider();

    isDead = false;
}