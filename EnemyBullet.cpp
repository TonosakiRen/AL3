#include "Mymath.h"
#include "EnemyBullet.h"
#include <assert.h>
void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("dog.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	velocity_ = velocity;
}
void EnemyBullet::Update() {

	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}
void EnemyBullet::Draw(const ViewProjection& viewProjection) {

	// モデル描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}