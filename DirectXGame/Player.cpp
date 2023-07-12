#include "Player.h"
#include <assert.h>
/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="textureHandle">テクスチャハンドル</param>
void Player::Initialize(Model* model) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
}

void Player::Update() { 
	
	worldTransform_.translation_ = {0.0f, 3.0f, 0.0f};
	worldTransform_.UpdateMatrix();
}
void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}