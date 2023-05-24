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
	case Phase::approach:
	default:
		PhaseApproach();
	case Phase::Leave:
		PhaseLeave();
		break;
	}

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
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	/*if (worldTransform_.translation_.z > -10.0f) {
		phase_ = Phase::Leave;
	}*/
}

void Enemy::PhaseLeave() 
{
	worldTransform_.translation_ = Subtract(worldTransform_.translation_, velocity_);
	if (worldTransform_.translation_.z < 10.0f) {
		phase_ = Phase::approach;
	}
}
