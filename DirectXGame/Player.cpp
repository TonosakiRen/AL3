#include "Player.h"
#include <assert.h>
#include "Mymath.h"
#include <numbers>
#include "ImGui.h"
#include "GlobalVariables.h"
#include "WinApp.h"
#include <algorithm>
#include "Audio.h"
   
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

	// GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// const char* groupName = "Player";
	//// グループを追加
	// GlobalVariables::GetInstance()->CreateGroup(groupName);
	// globalVariables->AddItem(
	//     groupName, "floatingSpeed", floatingSpeed);

	// ApplyGlobalVariables();
	uint32_t textureReticle = TextureManager::Load("reticle.png");
	sprite2DReticle_.reset(
	    Sprite::Create(textureReticle, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	sprite2DReticle_->SetSize({64.0f, 64.0f});

	BaseCharacter::Initialize(models);
	worldTransform_.translation_ = {0.0f, 1.0f, -20.0f};
	playerDirection = Normalize({0.0f,0.0f,1.0f});
	modelWorldTransform_[static_cast<int>(Player::Parts::Body)].Initialize();
	modelWorldTransform_[static_cast<int>(Player::Parts::Body)].parent_ = &worldTransform_;

	for (int i = 0; i < bulletNum; i++) {
		bullet[i].worldTransform.scale_ = {0.3f, 0.3f, 0.3f};
		bullet[i].worldTransform.Initialize();
	}

	audio = Audio::GetInstance();
	hitSoundHandle_ = audio->LoadWave("hitPlayer.wav");
	shotSoundHandle_ = audio->LoadWave("shot.wav");


}

void Player::Initialize2() {
	worldTransform_.translation_ = {0.0f, 1.0f, -20.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	playerDirection = Normalize({0.0f, 0.0f, 1.0f});
	for (int i = 0; i < bulletNum; i++) {
		bullet[i].isActive = false;
	}
	isFocus = false;
	preRightBotton = false;

	bulletCooltime = 0;
	
	bulletRange = 30.0f;

	hitCoolTime = 0;
	isHit = false;
	hitTranslation = {0.0f, 0.0f, 0.0f};
	hp = 6;
	isDead = false;
}

void Player::Update() { 

	hitCoolTime--;

	// ゲームパッドの状態
	XINPUT_STATE joyState;
	// gamePadが有効なら
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (hitCoolTime <= 30) {
			// 速さ
			const float speed = 0.2f;
			// 移動量
			move = {
			    (float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
			    (float)joyState.Gamepad.sThumbLY / SHRT_MAX};
			// 移動量に速さを反映
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
				move = Normalize(move) * (speed * 0.5f);
			} else {
				move = Normalize(move) * speed;
			}
			

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

		} else if (hitCoolTime >= 52) {
				const float hitSpeed = 1.0f;
				Vector3 hitToPlayer = LoseY(Normalize(worldTransform_.translation_ - hitTranslation));
				move = hitToPlayer * hitSpeed;
				// 移動
				worldTransform_.translation_ = worldTransform_.translation_ + move;
		}

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
			Vector3 toFocus = LoseY(focus_->translation_ - worldTransform_.translation_);
			OrientVector(worldTransform_.rotation_, toFocus);

			Matrix4x4 matViewport =
			    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
			Matrix4x4 matViewProjectionViewport =
			    viewProjection_->matView * viewProjection_->matProjection * matViewport;

			Vector3 positionReticle = Transform(focus_->translation_, matViewProjectionViewport);

			sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

			bulletCooltime--;
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X && bulletCooltime <= 0) {
				for (int i = 0; i < bulletNum; i++) {
					if (bullet[i].isActive == false) {
						audio->PlayWave(shotSoundHandle_,false,0.2f);
						bulletCooltime = 10;
						bullet[i].isActive = true;
						bullet[i].direction = Normalize(toFocus);
						bullet[i].worldTransform.translation_ = worldTransform_.translation_;
						bullet[i].startPosition = worldTransform_.translation_;
						OrientVector(bullet[i].worldTransform.rotation_,bullet[i].direction);
						break;
					}
				}
			}


		}

		for (int i = 0; i < bulletNum; i++) {
			if (bullet[i].isActive == true) {
				bullet[i].worldTransform.translation_ += bullet[i].direction * bulletSpeed;
				bullet[i].worldTransform.UpdateMatrix();
				if (Length(bullet[i].startPosition - bullet[i].worldTransform.translation_) >= bulletRange) {
					bullet[i].isActive = false;
				}
			}
		}

	}

	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -49.0f, 49.0f);
	worldTransform_.translation_.z = std::clamp(worldTransform_.translation_.z, -49.0f, 49.0f);
	worldTransform_.UpdateMatrix();
	modelWorldTransform_[static_cast<int>(Player::Parts::Body)].UpdateMatrix();

	
	if (hp <= 0) {
		isDead = true;
	}
}

void Player::Draw(const ViewProjection& viewProjection) {
	int index = 0;
	for (auto model : models_) {
		model->Draw(modelWorldTransform_[index], viewProjection);
		index++;
	}
	for (int i = 0; i < bulletNum; i++) {
		if (bullet[i].isActive == true) {
			models_[0]->Draw(bullet[i].worldTransform, viewProjection);
		}
	}
}

void Player::DrawUI() {
	if (isFocus) {
		sprite2DReticle_->Draw();
	}
}