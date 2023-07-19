#include "RailCamera.h"


void RailCamera::Initialize(const WorldTransform& worldTransform) { 

	viewProjection_.Initialize();
}

void RailCamera::Update() { 
	worldTransform_.translation_.z -= 0.5f;
	worldTransform_.UpdateMatrix();
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
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
