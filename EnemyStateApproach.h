#pragma once
#include "BaseEnemyState.h"
#include "TimeCall.h"
#include <memory>
class EnemyStateApproach : public BaseEnemyState {
public:
	void Initialize(Enemy* pEnemy) override;
	void Update() override;
	void FireAndReset();

private:
	// 時限発動リスト
	std::list<std::unique_ptr<TimeCall>> timeCalls_;
};
