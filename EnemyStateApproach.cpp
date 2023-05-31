#include "EnemyStateApproach.h"
#include "EnemyStateLeave.h"
#include "Enemy.h"

void EnemyStateApproach::Initialize(Enemy* pEnemy) {
	// 発射タイマーを初期化
	pEnemy_ = pEnemy;
	FireAndReset();
}

void EnemyStateApproach::Update() {
	
	for (const std::unique_ptr<TimeCall>& timeCall : timeCalls_) {
		timeCall->Update();
	}

	// 移動(ベクトルを加算)
	pEnemy_->move(pEnemy_->GetApproachVelocity());
	// 既定の位置に到達したら離脱
	if (pEnemy_->GetTransforom().translation_.z < 0.0f) {
		std::unique_ptr<EnemyStateLeave> enemyStateLeave = std::make_unique<EnemyStateLeave>();
		pEnemy_->ChangeState(std::move(enemyStateLeave));
	}
}

void EnemyStateApproach::FireAndReset() {
	//発射
	pEnemy_->Fire();
	// 発射タイマーを初期化
	timeCalls_.push_back(
	    std::make_unique<TimeCall>(std::bind(&EnemyStateApproach::FireAndReset, this), Enemy::kFireInterval));
}
