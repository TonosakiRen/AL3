#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <cstdint>

class Player;

/// <summary>
/// 自キャラの玉
/// </summary>
class EnemyBullet {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity, Player* pPlayer);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	// 衝突したら呼び出されるコールバック関数
	void OnCollision();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }
	void SetPlayer(Player* player) { player_ = player; }
	Vector3 GetWorldPosition() { return worldTransform_.translation_; }
	float GetRadius() const { return radius_; }

private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 velocity_;
	// 寿命
	static const int32_t kLifeTime = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
	//プレイヤーポインタ
	Player* player_;
	// 当たり判定
	const float radius_ = 1.0f;
};
