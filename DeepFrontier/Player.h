#pragma once
#include "CharacterBase.h"
#include "animationManager.h"
#include "InputManager.h"
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
	int actionTimer;
	float verticalVelocity;
	bool isJumping;

public:
	Player();
	virtual ~Player();

	void Init() override;
	void Update(const InputManager& inputManager);
	void UpdateCollider();
	void Draw() override;
	void Release() override;

};
