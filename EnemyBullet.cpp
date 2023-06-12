#include "Mymath.h"
#include "EnemyBullet.h"
#include <assert.h>
#include "Player.h"
void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity,Player* pPlayer) {
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
	SetPlayer(pPlayer);
	worldTransform_.UpdateMatrix();
}
void EnemyBullet::Update() {

	//敵弾から自キャラへのベクトルを計算
	Vector3 toPlayer = player_->GetWorldPosition() - worldTransform_.translation_; 

	//ベクトルを正規化する
	toPlayer = Normalize(toPlayer);
	velocity_ = Normalize(velocity_);
	//球面線形補間により、今の速度と自キャラベクトルを内挿し、新たな速度とする	
	velocity_ = Slerp(velocity_, toPlayer, 0.2f) * 0.5f;
	//進行方向に見た目の回転を合わせる(ex1)
	// Y軸周り角度(θy)
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

	Matrix4x4 tmp = MakeRotateYMatrix(-std::atan2(velocity_.x, velocity_.z));
	Vector3 velocityZ = Transform(velocity_, tmp);

	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);

	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::OnCollision() { isDead_ = true; }

void EnemyBullet::Draw(const ViewProjection& viewProjection) {

	// モデル描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}