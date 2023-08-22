#pragma once
#include "BaseCharacter.h"
#include "Model.h"
#include "WorldTransform.h"
#include <cstdint>
#include "ViewProjection.h"
#include "Input.h"
#include <vector>
#include <optional>
#include "Sprite.h"
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

	void DrawUI();

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	enum class Parts { Body,PartsNum };

	Vector3 GetDirection() { return playerDirection; }

	WorldTransform* GetFocus() { return focus_; }
	void SetFocus(WorldTransform* focus) { focus_ = focus; }
	bool GetIsFocus() { return isFocus; }

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

	//焦点相手
	WorldTransform* focus_ = nullptr; 

	bool isFocus = false;
	bool preRightBotton = false;


 private:
	std::unique_ptr<Sprite> sprite2DReticle_ = nullptr;
	
};