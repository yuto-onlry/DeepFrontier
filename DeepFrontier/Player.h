#pragma once
#include "CharacterBase.h"

class Player : public CharacterBase
{
public:
	Player();
	virtual ~Player();

	void Init() override;
	void Update() override;
	void Draw() override;

};
