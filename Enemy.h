#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <cstdint>
/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
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
	/// 接近
	/// </summary>
	void Approach();

	/// <summary>
	/// 離脱
	/// </summary>
	void Leave();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

private:

	//行動フェーズ
	enum class Phase {
		Approach, //接近
		Leave, //離脱する
	};
	//フェーズ
	Phase phase_ = Phase::Approach;

	static void (Enemy::*spFuncTable[])();

	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 approachVelocity_ = {0,0,-0.1f};
	Vector3 leaveVelocity_ = {-0.1f,0.1f,-0.1f};
	// デスフラグ
	bool isDead_ = false;
};
