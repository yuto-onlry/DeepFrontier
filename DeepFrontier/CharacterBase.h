#pragma once
#include "DxLib.h"
#include "CapsuleCollider.h"

class CharacterBase
{
protected:
	VECTOR position;	//キャラクター位置
	VECTOR velocity;	//キャラクター速度
	int hp;				//キャラクターの体力
	int maxHp;			//最大HP
	int attack;			//キャラクターの攻撃力
	int defense;		//キャラクターの防御力
	int modelHandle;	//キャラクターモデルのハンドル

	bool isDead;		//キャラクターの死亡状態

protected:
	CapsuleCollider capsuleCollider;
public:
	CharacterBase();		
	virtual ~CharacterBase();
public:
	CapsuleCollider* GetCapsuleCollider();
	virtual void Init();
	virtual void Update();
	virtual void Draw();
	virtual void Release();

	virtual void Damage(int damage);
	bool IsDead() const;
public:
	VECTOR GetPosition() const;
	int GetHp() const;
	int GetMaxHp() const;
public:
	void SetPosition(VECTOR pos);


};