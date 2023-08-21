#pragma once
#include "BaseCharacter.h"
#include "Model.h"
#include "WorldTransform.h"
#include <cstdint>
#include "ViewProjection.h"
#include "Input.h"
#include <vector>
#include <optional>
/// <summary>
/// 自キャラ
/// </summary>
class Player : public BaseCharacter {
public:

	////調整項目の適用
	//void ApplyGlobalVariables();

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

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	enum class Parts { Body,PartsNum };

	Vector3 GetDirection() { return playerDirection; }

private:

	

	// ワールド変換データ
	WorldTransform modelWorldTransform_[static_cast<int>(Player::Parts::PartsNum)];

	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	
	Vector3 playerDirection = {0.0f, 0.0f, 0.0f};

	enum class Behavior {
		kRoot,
		kAttack
	};
	Behavior behavior_ = Behavior::kRoot;

	//次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

private:

	
};