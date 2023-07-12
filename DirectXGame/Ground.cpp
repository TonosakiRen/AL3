#include "Ground.h"
#include <assert.h>

/// <summary>
/// 初期化
/// </summary>
void Ground::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);

	model_ = model;

	worldTransform_.Initialize();

	worldTransform_.scale_ = {50.0f, 1.0f, 50.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};

	worldTransform_.UpdateMatrix();
}

/// <summary>
/// 更新
/// </summary>
void Ground::Update() {}

/// <summary>
/// 描画
/// </summary>
void Ground::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}