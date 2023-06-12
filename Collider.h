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

private:
	//衝突半径
	float radius_ = 1.0f;
};
