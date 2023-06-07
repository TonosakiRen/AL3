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

	//Z方向に延びた形状
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;

	//Y軸周り角度(θy)
	worldTransform_.rotation_.y = std::atan2(velocity_.z, velocity_.x);
	
	Matrix4x4 tmp = MakeRotateYMatrix(-std::atan2(velocity_.z, velocity_.x));
	Vector3 velocityZ = Transform(velocity_, tmp);

	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);

	velocity_ = velocity;

	worldTransform_.UpdateMatrix();
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