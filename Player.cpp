#include "Player.h"
#include <assert.h>
#include "Mymath.h"
#include <imgui.h>
#include <algorithm>
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
	//キャラクターの移動ベクトル	
	Vector3 move = {0,0,0};

	//キャラクターの移動速さ
	const float kCharacterSpeed = 2.0f;

	//移動限界座標
	const float kMoveLimitX = 33;
	const float kMoveLimitY = 18;


	worldTransform_.scale_ = Vector3{1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = Vector3{0.0f, 0.0f, 0.0f};

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

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	ImGui::Begin("a");
	ImGui::Text(
	    "Player.pos x %.3f y %.3f z %.3f,", worldTransform_.translation_.x,
	    worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::SliderFloat3("Player.pos",&worldTransform_.translation_.x,0.0f,1.0f);
	ImGui::End();

}
void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}