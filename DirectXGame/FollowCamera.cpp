#include "FollowCamera.h"
#include "Input.h"
#include <numbers>
#include "ImGuiManager.h"
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
		goalRotation.y += (float)joyState.Gamepad.sThumbRX * rotateSpeed;

		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER && moveDirection_ == false) {
			moveDirection_ = true;
			direction_ = player_->GetDirection();
		}

	}

	if (moveDirection_ == true) {
		
	}

	if (player_->GetIsFocus() == true) {
		// 追従対象がいれば
		if (target_) {
		
			// 追従対象からカメラまでのオフセット
			Vector3 offset = {offset_};

			// カメラの角度から回転行列を計算する
			Matrix4x4 rotateMatrix = MakeRotateYMatrix(player_->GetWorldTransform().rotation_.y);

			// オフセットをカメラの回転に合わせて回転させる
			offset = TransformNormal(offset, rotateMatrix);

			// 座標をコピーしてオフセット分ずらす
			goalTranslation = target_->translation_ + offset;
			
			Vector3 toPlayer =
			    Normalize(player_->GetWorldTransform().translation_ - goalTranslation);
			Vector3 toFocus = player_->GetFocus()->translation_ - goalTranslation;

			Vector3 focus = Slerp(toPlayer, toFocus, 0.8f);

			OrientVector(goalRotation, focus);

			ImGui::Begin("pl");
			ImGui::DragFloat("rortaitoneppe", &rataionspeed,Radian(0.1f));
			ImGui::DragFloat("moveoneppe", &movespeed, 0.1f);
			ImGui::End();

			closeVector3(viewProjection_.rotation_, goalRotation, rataionspeed);
			closeVector3(viewProjection_.translation_, goalTranslation, movespeed);
			
		}

		
	} else {
		// 追従対象がいれば
		if (target_) {


			// 追従対象からカメラまでのオフセット
			Vector3 offset = {0.0f, 4.0f, -12.0f};
			goalRotation.x = 15.0f * std::numbers::pi_v<float> / 180.0f;

			// カメラの角度から回転行列を計算する
			Matrix4x4 rotateMatrix = MakeRotateYMatrix(goalRotation.y);

			// オフセットをカメラの回転に合わせて回転させる
			offset = TransformNormal(offset, rotateMatrix);
			// 座標をコピーしてオフセット分ずらす
			goalTranslation = target_->translation_ + offset;

			closeVector3(viewProjection_.rotation_, goalRotation, rataionspeed);
			closeVector3(viewProjection_.translation_, goalTranslation, movespeed);
		}

		
	
	}


	
	viewProjection_.UpdateMatrix();
}