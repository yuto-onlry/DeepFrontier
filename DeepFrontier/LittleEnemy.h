#pragma once
#include "EnemyBase.h"

class LittleEnemy : public EnemyBase
{
public:
	LittleEnemy();
	virtual ~LittleEnemy();

	void Init() override;	

};