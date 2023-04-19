#include"Player.h"

void Player::Initialize(Model* model, uint32_t textureHandlle) { 
	assert(model);
	model_ = model;
	textureHandle_ = textureHandlle;
	worldTransform_->Initialize();
}

void Player::Update() {}

void Player::Draw() {}
