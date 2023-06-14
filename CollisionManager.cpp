#include "CollisionManager.h"
#include "Collider.h"
#include "Mymath.h"
void CollisionManager::CheckAllCollision() {

	// リスト内のペアを総当たり
	std::vector<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;

		// イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::vector<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;
			// 衝突フィルタリング
			if ((colliderA->GetCollsionAttribute() & colliderB->GetCollisionMask()) == 0 ||
			    (colliderB->GetCollsionAttribute() & colliderA->GetCollisionMask()) == 0) {
				continue;
			}
			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	Vector3 posA = colliderA->GetWorldPosition();
	Vector3 posB = colliderB->GetWorldPosition();
	float distance = Length(posA - posB);
	// 球と球の交差判定
	if (distance <= colliderA->GetRadius() + colliderB->GetRadius()) {
		// コライダーAの衝突時コールバックを呼び出す
		colliderA->OnCollision();
		// コライダーBの衝突時コールバックを呼び出す
		colliderB->OnCollision();
	}
}