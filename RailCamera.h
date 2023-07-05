#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Mymath.h"
#include "CatmullRomSpline.h"
class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3& tranlation,Vector3& rotation);

	/// <summary>
	///更新 
	/// </summary>
	void Update();

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	void Draw();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//CatmullRomSpline
	CatmullRomSpline catmullRomSpline_;
	//注視点
	Vector3 target;
	//移動用フレーム
	uint32_t frame;
};
