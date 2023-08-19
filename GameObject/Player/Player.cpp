#include"Player.h"
#include<cassert>

void Player::Initialize(Model* model, uint32_t& textureHandle) { 
	assert(model);
	textureHandle_ = textureHandle;
	model_ = model;
	worldTransform_.Initialize();
}

void Player::Updete() { 
	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_,textureHandle_);

}
	

