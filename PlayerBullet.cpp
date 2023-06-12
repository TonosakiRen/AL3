#include "PlayerBullet.h"
#include "Mymath.h"
#include <assert.h>
void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	//NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.jpeg");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	velocity_ = velocity;
}
void PlayerBullet::Update() { 
	
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();
	
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::OnCollision() { isDead_ = true; }

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	//モデル描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}