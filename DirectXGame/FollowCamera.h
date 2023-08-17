#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Mymath.h"
/// <summary>
/// 追従カメラ
/// </summary>
class FollowCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

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
	//追従対象
	const WorldTransform* target_ = nullptr;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	float a = 0.0f;
};
