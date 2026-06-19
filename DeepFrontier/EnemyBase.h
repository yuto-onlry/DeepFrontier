#pragma once
#include "CharacterBase.h"
#include "AnimationManager.h"

class EnemyBase : public CharacterBase
{
protected:
	//アニメーション管理
	AnimationManager animationManager;
	float moveSpeed;	//移動速度
	float attackRange;	//攻撃範囲

public:
	EnemyBase();
	virtual ~EnemyBase();

	void Init() override;

	//playerの座標を受け取る
	virtual void Update(VECTOR playerPos);
	virtual void Release() override;
	virtual void Draw() override;

protected:
	void GetMovePlayerPos(VECTOR playerPos);
	bool GetAttackRange(VECTOR playerPos);
};