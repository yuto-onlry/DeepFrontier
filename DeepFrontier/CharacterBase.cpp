#include "CharacterBase.h"

CharacterBase::CharacterBase()
	: position({ 0, 0, 0 }),
	velocity({ 0, 0, 0 }),
	hp(100),
	maxHp(100),
	attack(10),
	defense(5),
	modelHandle(-1),
	isDead(false)
{
	capsuleCollider.SetOwner(this);
	capsuleCollider.SetActive(true);
}

CharacterBase::~CharacterBase()
{
	Release();
}
CapsuleCollider* CharacterBase::GetCapsuleCollider()
{
	return &capsuleCollider;
}
void CharacterBase::Init()
{

}

void CharacterBase::Update()
{
	position = VAdd(position, velocity);
}

void CharacterBase::Draw()
{
	if (modelHandle != -1)
	{
		MV1SetPosition(modelHandle, position);
		MV1DrawModel(modelHandle);
	}
}

void CharacterBase::Release()
{
	if (modelHandle != -1)
	{
		MV1DeleteModel(modelHandle);
		modelHandle = -1;
	}
}

void CharacterBase::Damage(int damage)
{
	if (isDead == true)
		return;
	int finalDamage = damage - defense;

	if (finalDamage < 1)
		finalDamage = 1;

	hp -= finalDamage;

	if (hp <= 0)
	{
		hp = 0;
		isDead = true;

		capsuleCollider.SetActive(false);
	}
}
bool CharacterBase::IsDead() const
{
	return isDead;
}

VECTOR CharacterBase::GetPosition() const
{
	return position;
}
int CharacterBase::GetHp() const
{
	return hp;
}

int CharacterBase::GetMaxHp() const
{
	return maxHp;
}
//
int CharacterBase::GetAttack() const
{
	return attack;
}
void CharacterBase::SetPosition(VECTOR pos)
{
	position = pos;
	capsuleCollider.SetPosition(position);
}