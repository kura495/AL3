#include "Enemy.h"
#include<cassert>

void Enemy::Initialize(Model* model) {
	// modelチェック
	assert(model);
	// モデル読み込み
	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("sample.png");
	// ワールドトランスフォーム初期化
	worldTransform_.Initialize();
}

void Enemy::Update() {

	switch(phase_) { 
	case Phase::Approach:
		PhaseApproach();
		break;
	case Phase::Leave:
		PhaseLeave();
		break;
	}
	ImGui::Begin("Enemy");
	float point[Vector3D] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("Point", point, -30.0f, 30.0f);
	ImGui::End();
	worldTransform_.translation_.x = point[x];
	worldTransform_.translation_.y = point[y];
	worldTransform_.translation_.z = point[z];

	worldTransformEx_.UpdateMatrix(
	    worldTransform_, worldTransform_.scale_, worldTransform_.rotation_,
	    worldTransform_.translation_);
}

void Enemy::Draw(const ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

//プライベート関数
void Enemy::Move() { 
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_); 
}

void Enemy::PhaseApproach() 
{
	if (worldTransform_.translation_.z < -30.0f) {
		phase_ = Phase::Leave;
	}
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	
}

void Enemy::PhaseLeave()
{
	if (worldTransform_.translation_.z > 30.0f) {
		phase_ = Phase::Approach;
	}
	worldTransform_.translation_ = Subtract(worldTransform_.translation_, velocity_);
	
}
