#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <cstdint>
#include "Collider.h"
/// <summary>
/// 自キャラの玉
/// </summary>
class PlayerBullet : public Collider{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	// 衝突したら呼び出されるコールバック関数
	void OnCollision() override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }
	float GetRadius() const { return radius_; }
	Vector3 GetWorldPosition() override { return worldTransform_.translation_; }
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

	private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_ = 0u;
	//速度
	Vector3 velocity_;
	//寿命
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
	// 当たり判定
	const float radius_ = 1.0f;
};
