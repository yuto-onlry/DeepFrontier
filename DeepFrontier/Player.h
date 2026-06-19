#pragma once
#include "CharacterBase.h"
#include "animationManager.h"
#include "InputManager.h"
#include  <dinput.h>

class Player : public CharacterBase
{
private:
	//ジョイスティックの状態
	DINPUT_JOYSTATE input;	
	//アニメーション管理
	AnimationManager animationManager; 
	int animModelHandle;
public:
	Player();
	virtual ~Player();

	void Init() override;
	void Update(const InputManager& inputManager);
	void Draw() override;
	void Release() override;

};
