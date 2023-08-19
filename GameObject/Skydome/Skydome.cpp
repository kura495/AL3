#include "GameObject/Skydome/Skydome.h"

void Skydome::Initialize(Model* model) { 
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix();
}

void Skydome::Update() {


}

void Skydome::Draw(const ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_);
}
