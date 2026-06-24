#pragma once
#include "DxLib.h"

class CharacterBase;

enum class ColliderTag
{
	None,
	Player,
	Enemy,
	PlayerAttack,
	EnemyAttack,
	Wall,
	Item
};
enum class ColliderType
{
	Box,
	Sphere,
	Capsule
};

class ColliderBase
{
protected:
	VECTOR position;
	ColliderTag tag;
	CharacterBase* owner;
	bool isActive;

public:
	ColliderBase();
	virtual ~ColliderBase();
public:
	virtual ColliderType GetColliderType() const = 0;
	virtual void DrawDebug() const = 0;
public:
	void SetPosition(const VECTOR& pos);
	void SetTag(ColliderTag setTag);
	void SetOwner(CharacterBase* setOwner);
	void SetActive(bool setActive);
public:
	VECTOR GetPosition() const;
	ColliderTag GetTag() const;
	CharacterBase* GetOwner() const;
	bool IsActive() const;

};
