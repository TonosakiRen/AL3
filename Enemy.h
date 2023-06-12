#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <cstdint>
#include "EnemyBullet.h"
#include "BaseEnemyState.h"
#include "EnemyStateApproach.h"
#include "EnemyStateLeave.h"

class Player;

/// <summary>
/// 敵
/// </summary>
class Enemy : public Collider{
public:
	//発射感覚
	static const int kFireInterval = 60;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Fire();

	/// <summary>
	/// 動く
	/// </summary>
	void move(Vector3 velocity);

	/// <summary>
	/// state変更
	/// </summary>
	void ChangeState(std::unique_ptr<BaseEnemyState> changeState);

	//衝突したら呼び出されるコールバック関数
	void OnCollision() override;

	// 弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }
	

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }
	int GetFireCount() const { return fireCount_; }
	void SetFireCount(int fireCount) { fireCount_ = fireCount; }
	WorldTransform GetTransforom() const { return worldTransform_; }
	Vector3 GetApproachVelocity() const { return approachVelocity_; }
	Vector3 GetLeaveVelocity() const { return leaveVelocity_; }
	void SetPlayer(Player* player) { player_ = player; }
	float GetRadius() const { return radius_; }
	Vector3 GetWorldPosition() override { return worldTransform_.translation_; }

private:

	std::unique_ptr<BaseEnemyState> state_;
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 approachVelocity_ = {0,0,-0.1f};
	Vector3 leaveVelocity_ = {-0.1f,0.1f,-0.1f};
	// デスフラグ
	bool isDead_ = false;
	//発射タイマー
	int32_t fireCount_ = 0;
	// 弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//自キャラ
	Player* player_ = nullptr;
	// 当たり判定
	const float radius_ = 1.0f;
};
