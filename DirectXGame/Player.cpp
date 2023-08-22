#include "Player.h"
#include <assert.h>
#include "Mymath.h"
#include <numbers>
#include "ImGui.h"
#include "GlobalVariables.h"
   
//void Player::ApplyGlobalVariables() { 
//	
//	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
//	const char* groupName = "Player";
//	floatingSpeed = globalVariables->GetFloatValue(groupName, "floatingSpeed");
//}

/// 初期化
/// </summary>
/// <param name="model">モデル</param>
void Player::Initialize(const std::vector<std::unique_ptr<Model>>& models) {

	//GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	//const char* groupName = "Player";
	//// グループを追加
	//GlobalVariables::GetInstance()->CreateGroup(groupName);
	//globalVariables->AddItem(
	//    groupName, "floatingSpeed", floatingSpeed);

	//ApplyGlobalVariables();
	uint32_t textureReticle = TextureManager::Load("reticle.png");
	sprite2DReticle_.reset(Sprite::Create(textureReticle, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	BaseCharacter::Initialize(models);
	worldTransform_.translation_ = {0.0f, 1.0f, 0.0f};
	playerDirection = Normalize({0.0f,0.0f,1.0f});
	modelWorldTransform_[static_cast<int>(Player::Parts::Body)].Initialize();
	modelWorldTransform_[static_cast<int>(Player::Parts::Body)].parent_ = &worldTransform_;
}

void Player::Update() { 

	// ゲームパッドの状態
	XINPUT_STATE joyState;
	// gamePadが有効なら
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 速さ
		const float speed = 0.2f;
		// 移動量
		Vector3 move = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX};
		// 移動量に速さを反映
		move = Normalize(move) * speed;

		Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
		move = TransformNormal(move, rotateMatrix);

		// Y軸周り角度(θy)
		if ((float)joyState.Gamepad.sThumbLX != 0.0f ||
		    (float)joyState.Gamepad.sThumbLY != 0.0f) {
			worldTransform_.rotation_.y = std::atan2(move.x, move.z);
			playerDirection = move;
		}

		// 移動
		worldTransform_.translation_ = worldTransform_.translation_ + move;


		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && isFocus == false && preRightBotton == false) {
			isFocus = true;
		} else if (
		    joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && isFocus == true &&
		    preRightBotton == false) {
			isFocus = false;
		}


		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER ) {
			preRightBotton = true;
		} else {
			preRightBotton = false;
		}
		

		if (isFocus == true) {
			Vector3 toFocus = focus_->translation_ - worldTransform_.translation_;
			OrientVector(worldTransform_.rotation_, toFocus);
		}

	}
	
	worldTransform_.UpdateMatrix();
	modelWorldTransform_[static_cast<int>(Player::Parts::Body)].UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection) {
	int index = 0;
	for (auto model : models_) {
		model->Draw(modelWorldTransform_[index], viewProjection);
		index++;
	}
}

void Player::DrawUI() { sprite2DReticle_->Draw(); }