#include "Player.h"
#include <assert.h>
#include <imgui.h>
#include <algorithm>
#include "Mymath.h"


Player::~Player() {

}

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="textureHandle">テクスチャハンドル</param>
void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	//シングルとインスタンスを取得する
	input_ = Input::GetInstance();
}



void Player::Update() { 

	//デスフラグの立った球を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		if (bullet->IsDead()) {
			return true;
		}
		return false;
	});

	//キャラクターの移動ベクトル	
	Vector3 move = {0,0,0};

	//キャラクターの移動速さ
	const float kCharacterSpeed = 2.0f;

	//移動限界座標
	const float kMoveLimitX = 33;
	const float kMoveLimitY = 18;

	//回転速さ[ラジアン　/  frame]
	const float kRotSpeed = 0.02f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

	worldTransform_.scale_ = Vector3{1.0f, 1.0f, 1.0f};

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed; 
	}
	

	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	
	worldTransform_.translation_ += move;
	worldTransform_.translation_.x = std::clamp(
	    worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y = std::clamp(
	    worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);

	worldTransform_.UpdateMatrix();
	Attack();

	//弾更新
	for (const std::unique_ptr<PlayerBullet>& bullet : bullets_) {

		bullet->Update();
	}

	ImGui::Begin("a");
	ImGui::Text(
	    "Player.pos x %.3f y %.3f z %.3f,", worldTransform_.translation_.x,
	    worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::SliderFloat3("Player.pos",&worldTransform_.translation_.x,0.0f,1.0f);
	ImGui::End();
}

/// <summary>
/// 攻撃
/// </summary>
void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {

		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		//弾を生成、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));

	}
}

void Player::Draw(ViewProjection& viewProjection) {

	//弾描画
	for (const std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 Player::GetWorldPosition() { 
	//ワールド座標を入れる
	Vector3 worldPos;
	//ワールド行列の変更移動成分を取得(ワールド座標)
	worldPos = worldTransform_.translation_;
	return worldPos;
}