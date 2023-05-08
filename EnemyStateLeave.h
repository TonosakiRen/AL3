#pragma once
#include "BaseEnemyState.h"

class EnemyStateLeave : public BaseEnemyState {
public:
	void Update(Enemy* pEnemy) override;
};
