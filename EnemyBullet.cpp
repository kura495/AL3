#include "EnemyBullet.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position) { 
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("sample.png");
	worldTransform_.translation_ = position;
}

void EnemyBullet::Update() {

}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {

}

void EnemyBullet::UpdateMatrix() { 
	worldTransformEx->UpdateMatrix(worldTransform_,worldTransform_.scale_,worldTransform_.rotation_,worldTransform_.translation_); 
}
