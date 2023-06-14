#include "Skydome.h"
#include <assert.h>

/// <summary>
/// 初期化
/// </summary>
void Skydome::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);

	model_ = model;

	worldTransform_.Initialize();

	worldTransform_.scale_ = {50.0f,50.0f,50.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};

	worldTransform_.UpdateMatrix();
}

/// <summary>
/// 更新
/// </summary>
void Skydome::Update() {

}

/// <summary>
/// 描画
/// </summary>
void Skydome::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}