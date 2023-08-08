#include "EnemyBullet.h"
#include"GameObject/Player/Player.h"
void EnemyBullet::Initialize(Model* model, const Vector3& position,const Vector3& velocity) { 
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("Black.png");
	worldTransform_.Initialize();
	/*worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;*/
	velocity_ = velocity;
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	float VelocityXZ = sqrt((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, VelocityXZ);
	SetCollisionAttribute(kCollitionAttributeEnemy);
	SetcollisionMask(~kCollitionAttributeEnemy);
}

void EnemyBullet::Update() { 
	//Homing();
	Move();
	worldTransform_.UpdateMatrix(); 

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void EnemyBullet::OnCollision() { 
	isDead_ = true; }

Vector3 EnemyBullet::GetWorldPosition() {
	Vector3 worldPos;
	worldTransform_.UpdateMatrix();
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void EnemyBullet::Move() {
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
}
void EnemyBullet::Homing() {
	// 玉のホーミング
	toPlayer = Subtract(player_->GetWorldPosition(), worldTransform_.translation_);
	toPlayer = Normalize(toPlayer);
	velocity_ = Normalize(velocity_);
	velocity_ = VectorSLerp(velocity_, toPlayer, 0.1f);
	velocity_.x *= 0.5f;
	velocity_.y *= 0.5f;
	velocity_.z *= 0.5f;
	// 玉の向き
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	float VelocityXZ = sqrt((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, VelocityXZ);
}

