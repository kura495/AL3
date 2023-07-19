﻿#include "RailCamera.h"


void RailCamera::Initialize(ViewProjection viewProjection) { 
	worldTransform_.translation_ = viewProjection.translation_;
	worldTransform_.rotation_ = viewProjection.rotation_;
	worldTransform_.scale_ = {1,1,1};
	viewProjection_.Initialize();
}

void RailCamera::Update() { 
	worldTransform_.translation_.z += 1.0f;
	worldTransform_.matWorld_ =
	    MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	viewProjection_.Map();
	ImGui::Begin("Camera");
	float transform[Vector3D] = {
	    viewProjection_.translation_.x, viewProjection_.translation_.y,
	    viewProjection_.translation_.z
	};
	float rotate[Vector3D] = {
	    viewProjection_.rotation_.x, viewProjection_.rotation_.y, viewProjection_.rotation_.z};
	ImGui::SliderFloat3("translation", transform, 0.0f, 1.0f);
	ImGui::SliderFloat3("rotation", rotate, 0.0f, 1.0f);
	ImGui::End();
}
