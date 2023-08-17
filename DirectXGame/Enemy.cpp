#include "Enemy.h"
#include<numbers>
void Enemy::Initialize(const std::vector<std::unique_ptr<Model>>& models) {
	BaseCharacter::Initialize(models);
	worldTransform_.translation_ = {0.0f, 1.0f, 0.0f};
	modelWorldTransform_[static_cast<int>(Enemy::Parts::Body)].Initialize();
	modelWorldTransform_[static_cast<int>(Enemy::Parts::Body)].parent_ = &worldTransform_;
}

void Enemy::Update() {

	moveRotate_ += 1.0f * std::numbers::pi_v<float> / 180.0f;

	const Matrix4x4 rotateVectorMatrix = MakeRotateYMatrix(moveRotate_);
	
	// 速さ
	const float speed = 0.3f;
	Vector3 move = {0.0f,0.0f,0.0f};
	move.z += 0.1f;

	// 移動量正規化
	move = Normalize(move) ;
	//回転を反映
	move = move * rotateVectorMatrix;
	//　移動量にスピードを追加
	move = move * speed;
	

	// Y軸周り角度(θy)
	worldTransform_.rotation_.y = std::atan2(move.x, move.z);

	// 移動
	worldTransform_.translation_ = worldTransform_.translation_ + move;

	worldTransform_.UpdateMatrix();
	modelWorldTransform_[static_cast<int>(Enemy::Parts::Body)].UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	int index = 0;
	for (auto model : models_) {
		model->Draw(modelWorldTransform_[index], viewProjection);
		index++;
	}
}