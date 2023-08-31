#include "Mymath.h"
#include "WorldTransform.h"
// スケール、回転、平行移動を合成して秒列を計算する
void WorldTransform::UpdateMatrix() {
	// スケール、回転、平行移動を合成して行列を計算する
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);

	// 親があれば親のワールド行列を掛ける
	if (parent_) {
		matWorld_ *= parent_->matWorld_;

		worldRotateMatrix_ = MakeRotateMatrix(rotation_) * MakeRotateMatrix(parent_->rotation_);
		worldScale_ = scale_ * parent_->scale_;
		worldTranslation_ =
		    parent_->translation_ + 
		    translation_ * MakeRotateMatrix(parent_->rotation_);
	} else {
		worldRotateMatrix_ = MakeRotateMatrix(rotation_);
		worldScale_ = scale_;
		worldTranslation_ = translation_;
	}

	

	// 定数バッファに転送する
	if (constBuff_) {
		TransferMatrix();
	}
}