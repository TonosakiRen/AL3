#pragma once
#include "BaseEnemyState.h"

class EnemyStateLeave : public BaseEnemyState {
public:
	void Update() override;
	void Initialize(Enemy* pEnemy) override;
};
