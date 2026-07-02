#pragma once
#include "CharacterBase.h"
#include "animationManager.h"
#include "InputManager.h"
#include "PlayerAction.h"
#include "SphereCollider.h"
#include "Weapon.h"

#include  <dinput.h>

class Player : public CharacterBase
{
public:
	enum class PlayerState
	{
		Idle,
		Walk,
		Run,
		Jump,
		Attack,
		Avoid,
		Damage,
		GetUp,
		Dead
	};
public:
	//ジョイスティックの状態
	DINPUT_JOYSTATE input;	
	//アニメーション管理
	AnimationManager animationManager; 
	PlayerState state;
	int animModelHandle;
	float verticalVelocity;
	bool isJumping;
private:
	PlayerAction playerAction;
	VECTOR forward;
	Weapon weapon;

public:
	Player();
	virtual ~Player();
public:
	void Init() override;
	void Update(const InputManager& inputManager);
	void UpdateCollider();
	void Draw() override;
	void Release() override;
public:
	SphereCollider* GetAttackCollider();
	void DisableAttackCollider();
};
