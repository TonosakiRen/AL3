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

	BaseCharacter::Initialize(models);
	worldTransform_.translation_ = {0.0f, 1.0f, 0.0f};
	playerDirection = Normalize({0.0f,0.0f,1.0f});
	modelWorldTransform_[static_cast<int>(Player::Parts::Body)].Initialize();
	modelWorldTransform_[static_cast<int>(Player::Parts::Body)].parent_ = &worldTransform_;
	modelWorldTransform_[static_cast<int>(Player::Parts::Head)].Initialize();
	modelWorldTransform_[static_cast<int>(Player::Parts::Head)].parent_ = &worldTransform_;
	modelWorldTransform_[static_cast<int>(Player::Parts::L_arm)].Initialize();
	modelWorldTransform_[static_cast<int>(Player::Parts::L_arm)].translation_ = {-1.0f, 0.7f, 0.0f};
	modelWorldTransform_[static_cast<int>(Player::Parts::L_arm)].parent_ = &worldTransform_;
	modelWorldTransform_[static_cast<int>(Player::Parts::R_arm)].Initialize();
	modelWorldTransform_[static_cast<int>(Player::Parts::R_arm)].translation_ = {1.0f, 0.7f, 0.0f};
	modelWorldTransform_[static_cast<int>(Player::Parts::R_arm)].parent_ = &worldTransform_;
	modelWorldTransform_[static_cast<int>(Player::Parts::Hammer)].Initialize();
	modelWorldTransform_[static_cast<int>(Player::Parts::Hammer)].translation_ = {0.0f, 0.0f, 0.0f};
	modelWorldTransform_[static_cast<int>(Player::Parts::Hammer)].parent_ = &worldTransform_;
	InitializeFloatingGimmick();
}

void Player::Update() { 
	
	if (behaviorRequest_) {
		//振る舞いを変更する
		behavior_ = behaviorRequest_.value();
		//各振る舞いごとの初期化を実行
		switch (behavior_) { 
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		}
		//振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			BehaviorRootUpdate();
			break;
		case Behavior::kAttack:
		    BehaviorAttackUpdate();
			break;
		}

	worldTransform_.UpdateMatrix();
	modelWorldTransform_[static_cast<int>(Player::Parts::Body)].UpdateMatrix();
	modelWorldTransform_[static_cast<int>(Player::Parts::Head)].UpdateMatrix();
	modelWorldTransform_[static_cast<int>(Player::Parts::R_arm)].UpdateMatrix();
	modelWorldTransform_[static_cast<int>(Player::Parts::L_arm)].UpdateMatrix();
	modelWorldTransform_[static_cast<int>(Player::Parts::Hammer)].UpdateMatrix();
}
void Player::InitializeFloatingGimmick() { 
	floatingParameter_ = 0.0f;
 }

void Player::BehaviorRootUpdate() {
	// ゲームパッドの状態
	XINPUT_STATE joyState;
	// gamePadが有効なら
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 速さ
		const float speed = 0.3f;
		// 移動量
		Vector3 move = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX};
		// 移動量に速さを反映
		move = Normalize(move) * speed;

		Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
		move = TransformNormal(move, rotateMatrix);

		// Y軸周り角度(θy)
		if ((float)joyState.Gamepad.sThumbLX != 0.0f || (float)joyState.Gamepad.sThumbLY != 0.0f) {
			worldTransform_.rotation_.y = std::atan2(move.x, move.z);
			playerDirection = move;
		}

		// 移動
		worldTransform_.translation_ = worldTransform_.translation_ + move;

		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			behaviorRequest_ = Behavior::kAttack;
		}
	}
	UpdateFloatingGimmick();
}

void Player::BehaviorAttackUpdate() {

	const float degreeSpeed = +6.0f;

	switch (attackPhase) { 
	
	case Up:
		degree -= degreeSpeed;
		hammerDegree -= degreeSpeed;
		worldTransform_.translation_ += playerDirection * 0.1f;
		if (hammerDegree <= 0.0f) {
			attackPhase = Down;
		}
		break;
	case Down:
		degree += degreeSpeed;
		hammerDegree += degreeSpeed;
		if (hammerDegree >= 90.0f) {
			behaviorRequest_ = Behavior::kRoot;
			hammerDegree = 0.0f;
		}
		break;
	}
	
	modelWorldTransform_[static_cast<int>(Player::Parts::R_arm)].rotation_.x = Radian(degree);
	modelWorldTransform_[static_cast<int>(Player::Parts::L_arm)].rotation_.x = Radian(degree);
	modelWorldTransform_[static_cast<int>(Player::Parts::Hammer)].rotation_.x = Radian(hammerDegree);
}

void Player::UpdateFloatingGimmick() {
	//浮遊移動のサイクル
	const uint16_t period = 60;
	//1フレームでのパラメータ加算値
	const float step = floatingSpeed * std::numbers::pi_v<float> / period;
	//パラメータを1ステップ分加算
	floatingParameter_ += step;
	//2πを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * std::numbers::pi_v<float>);
	//浮遊の振幅<m>
	const float floatingAmplitude = 0.05f;
	//浮遊を座票に反映
	worldTransform_.translation_.y = worldTransform_.translation_.y + std::sin(floatingParameter_) * floatingAmplitude;
	modelWorldTransform_[static_cast<int>(Player::Parts::L_arm)].rotation_.x = std::sin(floatingParameter_);
	modelWorldTransform_[static_cast<int>(Player::Parts::R_arm)].rotation_.x = std::sin(floatingParameter_);
}

void Player::BehaviorRootInitialize() { 
	InitializeFloatingGimmick(); 
}

// 攻撃行動初期化
void Player::BehaviorAttackInitialize() {
	attackPhase = Up;
	degree = -90.0f;
	hammerDegree = 90.0f;
	
}

void Player::Draw(const ViewProjection& viewProjection) {
	int index = 0;
	for (auto model : models_) {
		model->Draw(modelWorldTransform_[index], viewProjection);
		index++;
	}
}