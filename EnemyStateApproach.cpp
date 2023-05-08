#include "EnemyStateApproach.h"
#include "EnemyStateLeave.h"
#include "BaseEnemyState.h"
#include "Enemy.h"
void EnemyStateApproach::Update(Enemy* pEnemy) {
	pEnemy->SetFireCount(pEnemy->GetFireCount() - 1);

	if (pEnemy->GetFireCount() <= 0) {
		// 弾を発射
		pEnemy->Fire();
		// 発射タイマーを初期化
		pEnemy->FireCountInitialize();
	}

	// 移動(ベクトルを加算)
	pEnemy->move(pEnemy->GetApproachVelocity());
	// 既定の位置に到達したら離脱
	if (pEnemy->GetTransforom().translation_.z < 0.0f) {
		std::unique_ptr<EnemyStateLeave> enemyStateLeave = std::make_unique<EnemyStateLeave>();
		pEnemy->ChangeState(std::move(enemyStateLeave));
	}
}
