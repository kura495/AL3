#include "GameObject/Skydome/Skydome.h"

void Skydome::Initialize(Model* model) { 
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_.x = 50.0f;
	worldTransform_.scale_.y = 50.0f;
	worldTransform_.scale_.z = 50.0f;
	worldTransform_.UpdateMatrix();
}

void Skydome::Update() {


}

void Skydome::Draw(const ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_);
}
