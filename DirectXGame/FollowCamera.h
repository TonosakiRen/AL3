#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Mymath.h"
#include "Player.h"
/// <summary>
/// 追従カメラ
/// </summary>
class FollowCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Player* player);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	void SetTarget(const WorldTransform* target) { target_ = target; }

private:

	Player* player_ = nullptr;

	//追従対象
	const WorldTransform* target_ = nullptr;
	//追従対象向き
	Vector3 direction_;
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//追従対象の向きに移動
	bool moveDirection_ = false;

	float a = 0.0f;

	bool isFocus = false;
	float focusMoveT = 0.0f;
};
