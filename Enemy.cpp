#include "Enemy.h"
#include "Mymath.h"
#include <assert.h>
void Enemy::Initialize(Model* model, const Vector3& position) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("dog.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

}

void (Enemy::*Enemy::spFuncTable[])() = {
    &Enemy::Approach,
    &Enemy::Leave,
};


void Enemy::Update() {

	(this->*spFuncTable[static_cast<size_t>(phase_)])();

	worldTransform_.UpdateMatrix();

}
void Enemy::Approach() {
	// 移動(ベクトルを加算)
	worldTransform_.translation_ += approachVelocity_;
	// 既定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}
void Enemy::Leave() {
	// 移動(ベクトルを加算)
	worldTransform_.translation_ += leaveVelocity_;
}
void Enemy::Draw(const ViewProjection& viewProjection) {
	// モデル描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}