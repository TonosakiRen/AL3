#include "Enemy.h"
#include<numbers>
#include "Player.h"
#include "Randam.h"
#include "Easing.h"
#include<algorithm>
#include "ImGuiManager.h"
void Enemy::Initialize(const std::vector<std::unique_ptr<Model>>& models) {
	BaseCharacter::Initialize(models);
	modelWorldTransform_[static_cast<int>(Enemy::Parts::Body)].scale_ = {3.0f, 3.0f, 3.0f};
	worldTransform_.translation_ = {0.0f, 3.0f, 0.0f};
	modelWorldTransform_[static_cast<int>(Enemy::Parts::Body)].Initialize();
	modelWorldTransform_[static_cast<int>(Enemy::Parts::Right)].Initialize();
	modelWorldTransform_[static_cast<int>(Enemy::Parts::Left)].Initialize();
	modelWorldTransform_[static_cast<int>(Enemy::Parts::Body)].parent_ = &worldTransform_;
	modelWorldTransform_[static_cast<int>(Enemy::Parts::Right)].parent_ = &worldTransform_;
	modelWorldTransform_[static_cast<int>(Enemy::Parts::Left)].parent_ = &worldTransform_;

	for (int i = 0; i < bulletNum; i++) {
		bullet[i].worldTransform.scale_ = {3.0f, 3.0f, 3.0f};
		bullet[i].worldTransform.Initialize();
	}
	audio = Audio::GetInstance();
	hitSoundHandle_ = audio->LoadWave("hit.wav");
}

void Enemy::Initialize2() {

	worldTransform_.translation_ = {0.0f, 3.0f, 0.0f};
	modelWorldTransform_[static_cast<int>(Enemy::Parts::Right)].translation_ = {0.0f, 0.0f, 0.0f};
	stats_ = normal;
	preStats_ = normal;
	for (int i = 0; i < bulletNum; i++) {
		bullet[i].isActive = false;
	}
	// 行動
	isRandam = false;
	randam = 0;
	// moveCoolTime
	isRotated = false;
	isRotate = false;
	isDecidePositive = false;
	isPositive = false;
	moveTime = 0;

	// Attack

	attackT = 0.0f;
	isSavePos = false;
	savePos = {0.0f, 0.0f, 0.0f};
	goalPos = {0.0f, 0.0f, 0.0f};
	attackCooltime = 60;

	// assault
	degree = 0.0f;
	degreeSpeed = 0.0f;
	isAssault = false;
	assaultTime = 0;
	isEndAssult = false;

	bulletCooltime = 0;
	bulletShotNum = 0;
	bulletShotAllNum = 0;
	bulletLastNum = 0;

	//
	isHit = false;
	hp = 300;
	isDead = false;
}

void Enemy::Update() {
	enemyToPlayer = LoseY(player_->GetWorldTransform().translation_ - worldTransform_.translation_);
	length = Length(enemyToPlayer);
	length = clamp(length, 0.0f, 10000.0f);
	if (isRandam == true) {
		isRandam = false;
		preStats_ = stats_;
		randam = Randam::RAND(0, 100);
		if (length <= 25.0f) {
			if (randam <= 50) {
				stats_ = normal;
			} else if (randam <= 90) {
				stats_ = attack;
			} else if (randam <= 95) {
				stats_ = bulletAttack;
			} else if (randam <= 100) {
				stats_ = assault;
			}
			
		} else if (length >= 35.0f) {
			if (randam <= 50) {
				stats_ = normal;
			} else if (randam <= 80) {
				stats_ = bulletAttack;
			} else if (randam <= 100) {
				stats_ = assault;
			}
		} else {
			if (randam <= 50) {
				stats_ = normal;
			} else if (randam <= 75) {
				stats_ = bulletAttack;
			} else if (randam <= 100) {
				stats_ = assault;
			}
		}

		if (preStats_ != normal) {
			stats_ = normal;
		}

		//初期化
		isRotated = false;
		isRotate = false;
		isDecidePositive = false;
		moveTime = 0;

		isSavePos = false;

		degree = 0.0f;
		degreeSpeed = 0.0f;
		isAssault = false;
		assaultTime = 0;
		isEndAssult = false;

		for (int i = 0; i < bulletNum; i++) {
		
			bullet[i].isActive = false;
			bullet[i].lifeSpan = 0;
			bullet[i].direction = {0.0f, 0.0f, 0.0f};
			bullet[i].worldTransform.translation_ = {0.0f, 0.0f, 0.0f};
		}

		bulletCooltime = 0;
		bulletShotNum = 0;
		bulletShotAllNum = 0;
		bulletLastNum = 0;
	}
	

	switch (stats_) {
	case Enemy::normal:
		
			
				enemyToPlayer = LoseY(player_->GetWorldTransform().translation_ - worldTransform_.translation_);
				goalRotation = {0.0f, 0.0f, 0.0f};
				OrientVector(goalRotation, enemyToPlayer);
				isRotate = closeRotate(worldTransform_.rotation_, goalRotation, Radian(3.0f));
			
			
			if (isRotate == true) {
				isRotated = true;
				worldTransform_.rotation_ = goalRotation;
			}

			if (isRotated == true) {
				moveTime++;
				if (isDecidePositive == false) {
					isPositive = Randam::RAND();
					isDecidePositive = true;
				}
				Vector3 move;
				if (isPositive == true) {
					move = {0.1f, 0.0f, 0.0f};
				} else {
					move = {-0.1f, 0.0f, 0.0f};
				}
				Matrix4x4 rotateMatrix = MakeRotateYMatrix(worldTransform_.rotation_.y);
				move = TransformNormal(move, rotateMatrix);
				worldTransform_.translation_ = worldTransform_.translation_ + move;
			}

			if (moveTime > 50) {
				isRandam = true;
			}
		
		
		break;
	case Enemy::attack:



		if (isSavePos == false) {
			isSavePos = true;
			savePos = worldTransform_.translation_;
			goalPos = {savePos};
			Matrix4x4 rotateMatrix = MakeRotateYMatrix(worldTransform_.rotation_.y);
			Vector3 goalMove = {0.0f, 0.0f, 8.0f};
			goalMove = TransformNormal(goalMove, rotateMatrix);
			goalPos = goalMove + savePos;
			attackCooltime = 60;
			attackT = 0.0f;
		}

		worldTransform_.translation_ =
		    Easing::easing(attackT, savePos, goalPos, attackSpeed, Easing::easeInQuint);
		modelWorldTransform_[static_cast<int>(Enemy::Parts::Right)].translation_ = Easing::Bezier(
		    {0.0f, 0.0f, 0.0f}, {12.0f, 0.0f, 3.0f}, {-5.0f, 0.0f, 12.0f}, attackT, attackSpeed,
		    Easing::easeInQuint,false);

		modelWorldTransform_[static_cast<int>(Enemy::Parts::Right)].scale_ = Easing::easing(
		    attackT, {1.0f, 1.0f, 1.0f}, {2.0f, 2.0f, 2.0f}, attackSpeed, Easing::easeInQuint);
	

		if (attackT >= 1.0f) {
			attackCooltime--;
		}

		if (attackCooltime < 0.0f) {
			modelWorldTransform_[static_cast<int>(Enemy::Parts::Right)].translation_ = {
			    0.0f, 0.0f, 0.0f};
			isRandam = true;
		}

		break;
	case Enemy::assault:
		//回転
		if (assaultTime >= 0) {
			degreeSpeed += Radian(2.0f);
			degreeSpeed = std::clamp(degreeSpeed, 0.0f, 5.8f);
			degree += degreeSpeed;
			if (degree >= 360.0f) {
				    degree = degree - 360.0f;
			}
		} else {	
			if (isEndAssult == false) {
			  degreeSpeed -= Radian(2.0f);
			  degreeSpeed = std::clamp(degreeSpeed, 0.0f, 5.8f);
			  degree += degreeSpeed;
			}
			if (degree >= 360.0f) {
			  isEndAssult = true;
			  degree = 0.0f;
			  isRandam = true;
			}
		}

		if (isAssault == false) {
			assaultTime = 120;
			goalRotation = {0.0f, 0.0f, 0.0f};
			OrientVector(goalRotation, enemyToPlayer);
			closeRotate(worldTransform_.rotation_, goalRotation, Radian(3.0f));
		}
		//移動
		if (degreeSpeed >= 5.8f) {
			isAssault = true;
		}
		if (isAssault == true && isEndAssult == false) {
			assaultTime--;
			Vector3 move = {0.0f, 0.0f, 1.1f};
			Matrix4x4 rotateMatrix = MakeRotateYMatrix(worldTransform_.rotation_.y);
			move = TransformNormal(move, rotateMatrix);
			worldTransform_.translation_ = worldTransform_.translation_ + move;
		}
		worldTransform_.rotation_.x = Radian(degree);
		break;
	case Enemy::bulletAttack:
		
		degree += Radian(40.0f);
		if (degree >= 360.0f) {
			degree = degree - 360.0f;
		}
		bulletCooltime--;
		if (bulletCooltime <= 0 && bulletShotAllNum < 6) {
			bulletShotNum = 0;
			for (int i = 0; i < bulletNum; i++) {
			  if (bullet[i].isActive == false) {
				  bullet[i].lifeSpan = 0;
				  bullet[i].isActive = true;
				  Matrix4x4 rotateMatrix = MakeRotateYMatrix(
					  worldTransform_.rotation_.y + 
					  Radian(90.0f * bulletShotNum) + Radian(2.0f * bulletShotAllNum));
				  bullet[i].direction = Vector3{1.0f, 0.0f, 0.0f} * rotateMatrix;
				  bullet[i].worldTransform.translation_ = worldTransform_.translation_;
				  OrientVector(bullet[i].worldTransform.rotation_, bullet[i].direction);
				  bulletShotNum++;
				  bulletLastNum++;
				  if (bulletShotNum == 4) {
					  bulletCooltime = 20;
					  bulletShotAllNum++;
					  break;
				  }
			  }
			}
		}

		for (int i = 0; i < bulletNum; i++) {
		  if (bullet[i].isActive == true) {
			  bullet[i].lifeSpan++;
			  bullet[i].worldTransform.translation_ += bullet[i].direction * bulletSpeed;
			  bullet[i].worldTransform.UpdateMatrix();
			  if (bullet[i].lifeSpan >= 240) {
				  bullet[i].isActive = false;
			  }
		  }
		}
		if (bullet[bulletLastNum - 1].isActive == false && bulletShotAllNum == 6) {
		  isRandam = true;
		}
		worldTransform_.rotation_.y =+ Radian(degree);
		break;
	default:
		break;
	}

	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -47.0f, 47.0f);
	worldTransform_.translation_.z = std::clamp(worldTransform_.translation_.z, -47.0f, 47.0f);
	worldTransform_.UpdateMatrix();
	modelWorldTransform_[static_cast<int>(Enemy::Parts::Body)].UpdateMatrix();
	modelWorldTransform_[static_cast<int>(Enemy::Parts::Right)].UpdateMatrix();
	modelWorldTransform_[static_cast<int>(Enemy::Parts::Left)].UpdateMatrix();

	if (isHit == true) {
		hp--;
		hp = std::clamp(hp, 0, 10000);
		audio->PlayWave(hitSoundHandle_, false, 0.2f);
		isHit = false;
	}
	if (hp <= 0) {
		isDead = true;
	}
}

void Enemy::Draw(const ViewProjection& viewProjection) {
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