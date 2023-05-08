#pragma once
#include "BaseEnemyState.h"
class EnemyStateApproach : public BaseEnemyState {
public:
	void Update(Enemy* pEnemy) override;
};
