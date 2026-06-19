#include "LittleEnemy.h"
#include "ModelPreset.h"

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

    position = VGet(300.0f, 100.0f, 300.0f);
    velocity = VGet(0.0f, 0.0f, 0.0f);

    moveSpeed = 1.5f;
    attackRange = 70.0f;

    modelHandle = ModelPreset::LoadLittleEnemyModel();

    MV1SetPosition(modelHandle, position);
    isDead = false;
}