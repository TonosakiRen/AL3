#pragma once
#include <vector>
class Collider;
class CollisionManager {
public:
	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollision();

	/// <summary>
	/// コライダー２つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	void SetColliders(const std::vector<Collider*>& colliders) { colliders_ = colliders; }
	void ClearColliders() { colliders_.clear(); }

private:
	std::vector<Collider*> colliders_;
};
