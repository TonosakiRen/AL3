#include "EnemyStateLeave.h"
#include "Enemy.h"
void EnemyStateLeave::Update() { 
	pEnemy_->move(pEnemy_->GetLeaveVelocity()); 
}
void EnemyStateLeave::Initialize(Enemy* pEnemy) { 
	pEnemy_ = pEnemy;
}
