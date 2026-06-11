#pragma once
#include "DxLib.h"

class CharacterBase
{
protected:
	VECTOR position;	//キャラクター位置
	VECTOR velocity;	//キャラクター速度

	int hp;				//キャラクターの体力
	int attack;			//キャラクターの攻撃力
	int defense;		//キャラクターの防御力
	int modelHandle;	//キャラクターモデルのハンドル

	bool isDead;		//キャラクターの死亡状態

public:
	CharacterBase();		
	virtual ~CharacterBase();

	virtual void Init();
	virtual void Update();
	virtual void Draw();
	virtual void Release();

	virtual void Damage(int damage);
	bool IsDead() const;

	VECTOR GetPosition() const;
	void SetPosition(VECTOR pos);


};