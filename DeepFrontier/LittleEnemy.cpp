#include "LittleEnemy.h"

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

    position = VGet(300.0f, 0.0f, 300.0f);
    velocity = VGet(0.0f, 0.0f, 0.0f);

    moveSpeed = 1.5f;
    attackRange = 70.0f;

    modelHandle = -1;
    isDead = false;
}