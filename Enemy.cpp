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

	FireCountInitialize();

	state_ = std::make_unique<EnemyStateApproach>();

}

//void (Enemy::*Enemy::spFuncTable[])() = {
//    &Enemy::Approach,
//    &Enemy::Leave,
//};

void Enemy::FireCountInitialize() {
	//発射タイマーを初期化
	fireCount_ = kFireInterval;
}


void Enemy::Update() {

	// デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		if (bullet->IsDead()) {
			return true;
		}
		return false;
	});

	/*(this->*spFuncTable[static_cast<size_t>(phase_)])();*/
	state_->Update(this);

	worldTransform_.UpdateMatrix();

	// 弾更新
	for (const std::unique_ptr<EnemyBullet>& bullet : bullets_) {

		bullet->Update();
	}

}

// <summary>
/// 攻撃
/// </summary>
void Enemy::Fire() {

		// 弾の速度
		const float kBulletSpeed = -1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成、初期化
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		// 弾を登録する
		bullets_.push_back(std::move(newBullet));
	
}

void Enemy::move(Vector3 velocity) { worldTransform_.translation_ += velocity; }

void Enemy::ChangeState(std::unique_ptr<BaseEnemyState> changeState) { 
	state_ = std::move(changeState); }

//void Enemy::Approach() {
//
//	
//	fireCount_--;
//
//	    if (fireCount_ <= 0) {
//		// 弾を発射
//		Fire();
//		// 発射タイマーを初期化
//		fireCount_ = kFireInterval;
//	    }
//
//	// 移動(ベクトルを加算)
//	worldTransform_.translation_ += approachVelocity_;
//	// 既定の位置に到達したら離脱
//	if (worldTransform_.translation_.z < 0.0f) {
//		/*phase_ = Phase::Leave;*/
//	}
//}
//void Enemy::Leave() {
//	// 移動(ベクトルを加算)
//	worldTransform_.translation_ += leaveVelocity_;
//}
void Enemy::Draw(const ViewProjection& viewProjection) {
	// 弾描画
	for (const std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
	// モデル描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}