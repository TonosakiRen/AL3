#include "EnemyStateLeave.h"
#include "Enemy.h"
void EnemyStateLeave::Update(Enemy* pEnemy) { 
	pEnemy->move(pEnemy->GetLeaveVelocity()); }
