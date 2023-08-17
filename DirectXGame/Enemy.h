#pragma once
#include "BaseCharacter.h"
#include "Model.h"
#include "WorldTransform.h"
#include <cstdint>
#include "ViewProjection.h"
#include <vector>
#include "Mymath.h"
class Enemy : public BaseCharacter {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(const std::vector<std::unique_ptr<Model>>& models) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(const ViewProjection& viewProjection) override;

	enum class Parts { Body, PartsNum };

	private:
	// ワールド変換データ
	WorldTransform modelWorldTransform_[static_cast<int>(Enemy::Parts::PartsNum)];

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	// 浮遊ギミックの媒介変数
	float moveRotate_ = 0.0f;
};
