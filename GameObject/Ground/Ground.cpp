#include "Ground.h"

void Ground::Initialize(Model* model) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix();
}

void Ground::Update() {}

void Ground::Draw(const ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_);
}