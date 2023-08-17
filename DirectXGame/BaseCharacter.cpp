#include "BaseCharacter.h"

void BaseCharacter::Initialize(const std::vector<std::unique_ptr<Model>>& models) {
	models_.clear();
	for (auto& model : models) {
		models_.emplace_back(model.get());
	}
	worldTransform_.Initialize();
}

void BaseCharacter::Update() { worldTransform_.UpdateMatrix(); }

void BaseCharacter::Draw(const ViewProjection& viewProjection) { 
	for (auto& model : models_) {
		model->Draw(worldTransform_, viewProjection);
	}
}