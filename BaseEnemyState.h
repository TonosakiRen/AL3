#pragma once
class Enemy;
class BaseEnemyState {
protected:
	Enemy* pEnemy_ = nullptr;
public:
	virtual void Update() = 0;
	virtual void Initialize(Enemy* pEnemy) = 0;
	virtual ~BaseEnemyState(){};
};
