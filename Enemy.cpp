#include "Enemy.h"
#include "Mymath.h"
#include <assert.h>
#include "Player.h"

void Enemy::Initialize(Model* model, const Vector3& position) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("dog.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	state_ = std::make_unique<EnemyStateApproach>();
	state_->Initialize(this);

}

void Enemy::Update() {

	// デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		if (bullet->IsDead()) {
			return true;
		}
		return false;
	});

	state_->Update();

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

	assert(player_);

	
	// 弾の速度
	const float kBulletSpeed = 1.0f;

	//弾の移動量
	Vector3 direction =
	    Normalize(player_->GetWorldPosition() - worldTransform_.translation_) * kBulletSpeed;

	Vector3 velocity(direction);

	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 弾を生成、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity , player_);

	// 弾を登録する
	bullets_.push_back(std::move(newBullet));
	
}

void Enemy::OnCollision() {
	// 何もしない
}

void Enemy::move(Vector3 velocity) { worldTransform_.translation_ += velocity; }

void Enemy::ChangeState(std::unique_ptr<BaseEnemyState> changeState) { 
	state_ = std::move(changeState);
	state_->Initialize(this);
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// 弾描画
	for (const std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
	// モデル描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}