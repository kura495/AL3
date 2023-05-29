#include "Enemy.h"
#include"Player.h"
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
	worldTransform_.translation_ = Add(worldTransform_.translation_, {10.0f, 0.0f, 0.0f});
	//最初の状態
	state_ = new PhaseApproach();
	ApproachInitialize();
}

void Enemy::Update() {
	state_->Update(this, velocity_);
	
	for(EnemyBullet* bullet_:bullets_) {
		bullet_->Update();
	}
	bullets_.remove_if([](EnemyBullet* bullet_) { 
		if (bullet_->Isdead()) {
			delete bullet_;
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

	worldTransformEx_.UpdateMatrix(
	    worldTransform_, worldTransform_.scale_, worldTransform_.rotation_,
	    worldTransform_.translation_);
}

void Enemy::Draw(const ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (EnemyBullet* bullet_ : bullets_) {
		bullet_->Draw(viewProjection);
	}
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
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
if (--BulletShotTimer <= 0) {
		Fire();
		BulletShotTimer = kFireInterval;
	}
}

void Enemy::Fire() {
	assert(player_);
	const float kBulletSpeed = 1.0f;
	Vector3 playerPosition=player_->GetWorldPosition();
	Vector3 enemyPosition=this->GetWorldPosition();
	Vector3 Bulletvelocity = Subtract(playerPosition, enemyPosition);
	Bulletvelocity = Normalize(Bulletvelocity);
	Bulletvelocity.x *= kBulletSpeed;
	Bulletvelocity.y *= kBulletSpeed;
	Bulletvelocity.z *= kBulletSpeed;
	BulletShotTimer = kFireInterval;
	EnemyBullet* newEnemyBullet_ = new EnemyBullet();
	
	newEnemyBullet_->Initialize(model_, worldTransform_.translation_, Bulletvelocity);
		bullets_.push_back(newEnemyBullet_);

}

void Enemy::ApproachInitialize() { 
	BulletShotTimer = kFireInterval;
}

void PhaseApproach::Update(Enemy* enemy,const Vector3&velocity) {
	    enemy->ApproachUpdate();
	if (enemy->GetTransform().z < -30.0f) {
		enemy->PhaseChange(new PhaseLeave());
	}
	enemy->WorldTransformAdd(velocity);
}

void PhaseLeave::Update(Enemy* enemy, const Vector3& velocity) {
	if (enemy->GetTransform().z > 30.0f) {
		enemy->PhaseChange(new PhaseApproach());
	}
	enemy->WorldTransformSubtract(velocity);
}
