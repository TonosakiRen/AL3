#pragma once
#include "Mymath.h"
/// <summary>
/// 衝突判定オブジェクトm
/// </summary>
class Collider {
public:
	//衝突時に呼ばれる関数
	virtual void OnCollision() = 0;


	//半径を取得
	float GetRadius() { return radius_; }
	//半径を設定
	void SetRadius(float radius) { radius_ = radius; }
	//ワールド座標を取得
	virtual Vector3 GetWorldPosition() = 0;

	//衝突属性(自分)を取得
	uint32_t GetCollsionAttribute() { return collisionAttribute_; }
	// 衝突属性(自分)を設定
	void SetCollisionAttribute(uint32_t attribute) { collisionAttribute_ = attribute; }
	// 衝突マスク(相手)を取得
	uint32_t GetCollisionMask() { return CollisionMask_; }
	// 衝突マスク(相手)を設定
	void SetCollisionMask(uint32_t attribute) { CollisionMask_ = attribute; }

private:
	//衝突半径
	float radius_ = 1.0f;
	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	//衝突マスク(相手)
	uint32_t CollisionMask_ = 0xffffffff;
};
