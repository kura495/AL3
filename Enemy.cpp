#include "Enemy.h"
#include<cassert>

Enemy::Enemy() {state_ = new PhaseApproach();}

Enemy::~Enemy() { delete state_; }

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
	StateUpdate();

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

void Enemy::StateUpdate() { 
	state_->Update(this);
}

void Enemy::Draw(const ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::PhaseChange(PhaseState* newState) { 
	delete state_;
	state_ = newState;
}



void PhaseApproach::Update(Enemy* enemy) {

	if (enemy->GetTransform().z < -30.0f) {
		enemy->PhaseChange(new PhaseLeave());
	}
}

PhaseApproach::PhaseApproach() {}

PhaseApproach::~PhaseApproach() {}

void PhaseLeave::Update(Enemy* enemy) {
	if (enemy->GetTransform().z > 30.0f) {
		enemy->PhaseChange(new PhaseApproach());
	}
}

PhaseLeave::PhaseLeave() {}

PhaseLeave::~PhaseLeave() {}

PhaseState::PhaseState() {}

PhaseState::~PhaseState() {}
