#include "Player.h"
#include <assert.h>
#include <imgui.h>
#include <algorithm>
#include "Mymath.h"
#include "CollisionConfig.h"
#include "WinApp.h"
#include "ViewProjection.h"


Player::~Player() { delete sprite2DReticle_; }

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="textureHandle">テクスチャハンドル</param>
void Player::Initialize(Model* model, uint32_t textureHandle,Vector3 playerPosition) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = playerPosition;
	//3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();
	//シングルとインスタンスを取得する
	input_ = Input::GetInstance();
	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	//衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributePlayer);
	//レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("reticle.png");
	//スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
}



void Player::Update(const ViewProjection& viewProjection) { 

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
	const float kCharacterSpeed = 0.2f;

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

	// 自機のワールド座標から3Dレティクルのワールド座標を計算
	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
	// 自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = {0.0f, 0.0f, 1.0f};
	// 自機のワールド行列の回転を反映
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	// ベクトルの長さを変える
	offset = Normalize(offset) * kDistancePlayerTo3DReticle;
	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = worldTransform_.translation_ + offset;
	worldTransform3DReticle_.UpdateMatrix();

	Vector3 positionReticle = worldTransform3DReticle_.translation_;
	//ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport =
	    viewProjection.matView * viewProjection.matProjection * matViewport;

	//ワールド->スクリーン座標変換(ここで3Dから2Dになる)
	positionReticle = Transform(positionReticle, matViewProjectionViewport);

	//スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

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
		Vector3 velocity = worldTransform3DReticle_.translation_ - worldTransform_.translation_;
		velocity = Normalize(velocity) * kBulletSpeed;

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		//弾を生成、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);
		//弾を登録する
		bullets_.push_back(std::move(newBullet));

	}
}

void Player::OnCollision() {
	//何もしない
}

void Player::DrawUI() { sprite2DReticle_->Draw(); }

void Player::Draw(ViewProjection& viewProjection) {

	//弾描画
	for (const std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	model_->Draw(worldTransform3DReticle_, viewProjection, textureHandle_);
}
