#pragma once
#include "CharacterBase.h"

class EnemyBase : public CharacterBase
{
protected
:
	float moveSpeed;	//移動速度
	float attackRange;	//攻撃範囲

public:
	EnemyBase();
	virtual ~EnemyBase();

	void Init() override;

	//playerの座標を受け取る
	virtual void Update(VECTOR playerPos);

	void Draw() override;

protected:
	void MovePlayer(VECTOR playerPos);
	bool AttackRange(VECTOR playerPos);
};