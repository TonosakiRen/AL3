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

	enum class Parts { Body, Head, L_arm, R_arm,Hammer, PartsNum };

private:

	

	// ワールド変換データ
	WorldTransform modelWorldTransform_[static_cast<int>(Player::Parts::PartsNum)];

	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	Vector3 playerDirection = {0.0f, 0.0f, 0.0f};

	float floatingSpeed = 0.0f;

	enum AttackPhase {
		Up,
		Down
	};

	AttackPhase attackPhase = Up;

	float degree = -90.0f;
	float hammerDegree = 90.0f;

	enum class Behavior {
		kRoot,
		kAttack
	};
	Behavior behavior_ = Behavior::kRoot;

	//次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

private:
	
	// 浮遊ギミック初期化
	void InitializeFloatingGimmick();

	// 浮遊ギミック更新
	void UpdateFloatingGimmick();

	// 通常行動更新
	void BehaviorRootUpdate();

	// 攻撃行動更新
	void BehaviorAttackUpdate();

	//通常行動初期化
	void BehaviorRootInitialize();

	// 攻撃行動初期化
	void BehaviorAttackInitialize();

};