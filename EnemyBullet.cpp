#include "EnemyBullet.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position,const Vector3& velosity) { 
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("Black.png");
	worldTransform_.Initialize();
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;
	worldTransform_.translation_ = position;
	velosity_=velosity;
}

void EnemyBullet::Update() { 
	Move();
	worldTransformEx.UpdateMatrix(worldTransform_,worldTransform_.scale_,worldTransform_.rotation_,worldTransform_.translation_); 

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void EnemyBullet::Move() {
	worldTransform_.translation_ = Add(worldTransform_.translation_, velosity_);
}


