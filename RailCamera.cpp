#include "RailCamera.h"
#include <imgui.h>
/// <summary>
/// 初期化
/// </summary>
void RailCamera::Initialize(Vector3& tranlation, Vector3& rotation) {
	//ワールドトランスフォームの初期設定
	worldTransform_.translation_ = tranlation;
	worldTransform_.rotation_ = rotation;
	worldTransform_.rotation_ = Vector3{1.0f,1.0f,1.0f};
	worldTransform_.UpdateMatrix();
	//ビュープロジェクション
	viewProjection_.farZ = 5000.0f;
	viewProjection_.Initialize();

	std::vector<Vector3> controlPoints_;
	controlPoints_ = {
	    {0.0f,  0.0f,  0.0f }, 
		 {0.0f, 0.0f, 1.0f}, 
	};
	catmullRomSpline_.Initialize(controlPoints_);
	frame = 0;
}

/// <summary>
/// 更新
/// </summary>
void RailCamera::Update() {
	const uint32_t kTargetDiffCount = 50;
	//frame++;
	if (frame == catmullRomSpline_.segmentCount) {
		frame = 0;
	}
	uint32_t targetFrame = frame + kTargetDiffCount;
	if (frame + kTargetDiffCount >= catmullRomSpline_.segmentCount) {
		targetFrame = targetFrame - frame;
	}
	target = catmullRomSpline_.points_[targetFrame];
	worldTransform_.translation_ = catmullRomSpline_.points_[frame];

	Vector3 targetDiff = target - worldTransform_.translation_;

	worldTransform_.rotation_ = LookAtOfEulerAngle(targetDiff);
	worldTransform_.matWorld_ = MakeAffineMatrix(  {1.0f, 1.0f, 1.0f}, worldTransform_.rotation_, worldTransform_.translation_);

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	//カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::Text("translation %f %f %f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::Text("roatation %f %f %f", worldTransform_.rotation_.x, worldTransform_.rotation_.y,worldTransform_.rotation_.z);
	ImGui::End();
}

void RailCamera::Draw() { }