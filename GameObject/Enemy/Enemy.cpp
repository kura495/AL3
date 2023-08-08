#include "Enemy.h"
#include"GameObject/Player/Player.h"
#include<cassert>
#include"scene/GameScene.h"

void Enemy::Initialize(Model* model,const Vector3& position) {
	// modelチェック
	assert(model);
	// モデル読み込み
	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("sample.png");
	// ワールドトランスフォーム初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = Add(worldTransform_.translation_, position);
	worldTransform_.UpdateMatrix();
	//最初の状態
	state_ = new PhaseApproach();
	ApproachInitialize();
	SetCollisionAttribute(kCollitionAttributeEnemy);
	SetcollisionMask(~kCollitionAttributeEnemy);
}

void Enemy::Update() {
	state_->Update(this, velocity_);
	for (TimedCall* timedCall_ : timedCalls_) {
		timedCall_->Update();
	}
	timedCalls_.remove_if([](TimedCall* timedCall_) {
		if (timedCall_->IsConpleted()) {
			delete timedCall_;
			return true;
		}
		return false;
	});
	

	ImGui::Begin("Enemy");
	float point[Vector3D] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("Point", point, -30.0f, 30.0f);
	ImGui::End();
	worldTransform_.translation_.x = point[x];
	worldTransform_.translation_.y = point[y];
	worldTransform_.translation_.z = point[z];

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;
	worldTransform_.UpdateMatrix();
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Enemy::OnCollision() { 
	isDead_ = true;
}

void Enemy::WorldTransformAdd(const Vector3& velocity) {
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity);
}

void Enemy::WorldTransformSubtract(const Vector3& velocity) {
	worldTransform_.translation_ = Subtract(worldTransform_.translation_, velocity);
}

void Enemy::PhaseChange(PhaseState* newState) { 
	delete state_;
	state_ = newState;
}

void Enemy::ApproachUpdate() {
	Fire();
	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::ApproachUpdate, this), 60));
}

void Enemy::Fire() {
	assert(player_);
	player_->GetWorldPosition();
	Vector3 toPlayer = Subtract(player_->GetWorldPosition(), worldTransform_.translation_);
	Vector3 velosity = Normalize(toPlayer);
	const float kBulletSpeed = 0.5f;
	velosity.x *= kBulletSpeed;
	velosity.y *= kBulletSpeed;
	velosity.z *= kBulletSpeed;
	EnemyBullet* newEnemyBullet_ = new EnemyBullet();
	newEnemyBullet_->Initialize(model_, worldTransform_.translation_, velosity);
	newEnemyBullet_->SetPlayer(player_);
	gameScene_->AddEnemyBullet(newEnemyBullet_);
}

void Enemy::ApproachInitialize() { 
	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::ApproachUpdate, this), 60));
}

void Enemy::LeaveInitialize() {
	    timedCalls_.remove_if([](TimedCall* timedCall_) {
		    if (timedCall_ != nullptr) {
			    delete timedCall_;
			    return true;
		    }
		    return false;
	    });
}

