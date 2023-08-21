#include "FollowCamera.h"
#include "Input.h"
#include <numbers>
void FollowCamera::Initialize(Player* player) { 
	// ビュープロジェクション
	viewProjection_.farZ = 5000.0f;
	viewProjection_.Initialize();
	player_ = player;
}
void FollowCamera::Update() {

	// ゲームパッドの状態
	XINPUT_STATE joyState;
	// gamePadが有効なら
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		const float rotateSpeed = 0.0000035f / (2.0f * float(M_PI));
		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX * rotateSpeed;

		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER && moveDirection_ == false) {
			moveDirection_ = true;
			direction_ = player_->GetDirection();
		}

		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && isFocus == false) {
			isFocus = true;
			focusMoveT = 0.0f;
		}
	}

	if (moveDirection_ == true) {
		
	}

	if (isFocus == true) {
		// 追従対象がいれば
		if (target_) {
			focusMoveT += 0.1f;
			focusMoveT = clamp(focusMoveT, 0.0f, 2.0f);
			// 追従対象からカメラまでのオフセット
			Vector3 offset = {focusMoveT, 10.0f, -20.0f};
			viewProjection_.rotation_.x = 15.0f * std::numbers::pi_v<float> / 180.0f;

			// カメラの角度から回転行列を計算する
			Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_.rotation_.y);

			// オフセットをカメラの回転に合わせて回転させる
			offset = TransformNormal(offset, rotateMatrix);
			// 座標をコピーしてオフセット分ずらす
			viewProjection_.translation_ = target_->translation_ + offset;
		}

		
	} else {
		// 追従対象がいれば
		if (target_) {
			// 追従対象からカメラまでのオフセット
			Vector3 offset = {0.0f, 10.0f, -20.0f};
			viewProjection_.rotation_.x = 15.0f * std::numbers::pi_v<float> / 180.0f;

			// カメラの角度から回転行列を計算する
			Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_.rotation_.y);

			// オフセットをカメラの回転に合わせて回転させる
			offset = TransformNormal(offset, rotateMatrix);
			// 座標をコピーしてオフセット分ずらす
			viewProjection_.translation_ = target_->translation_ + offset;
		}
	
	}


	
	viewProjection_.UpdateMatrix();
}