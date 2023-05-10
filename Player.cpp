#include <Player.h>
#include<cassert>
void Player::Initialize(Model* model, uint32_t textureHandle) { 
	assert(model);
	textureHandle_ = textureHandle;
	model_ = model;
	worldTransform_.Initialize();
	input_ = Input::GetInstance();
}

void Player::Updete() { 
	
	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	float theta = 0.0f;
	//キャラクターの移動の速さ
	const float CharacterSpeed = 0.2f;
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= CharacterSpeed;
	} else if(input_->PushKey(DIK_RIGHT)) {
		move.x += CharacterSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += CharacterSpeed;
	} else if(input_->PushKey(DIK_DOWN)) {
		move.y -= CharacterSpeed;
	}
	//スケール変換
	Matrix4x4 Scale_=MakeScaleMatrix(worldTransform_.scale_);
	//ローテーション変換
	Matrix4x4 RotateX_ = MakeRotateXMatrix(theta);
	Matrix4x4 RotateY_ = MakeRotateYMatrix(theta);
	Matrix4x4 RotateZ_ = MakeRotateZMatrix(theta);
	Matrix4x4 Rotate_ = Multiply(RotateX_, Multiply(RotateY_, RotateZ_));
	//トランスフォーム変換
	Matrix4x4 Transform_ = MakeScaleMatrix(worldTransform_.translation_);
	Matrix4x4 Move_ = MakeScaleMatrix(move);
	Transform_=Add(Transform_,Move_);
	//worldTransform_.matWorld_ = MakeAffineMatrix(Scale_, Rotate_, Transform_);
	//行列を定数バッファに送る
	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_,textureHandle_);

}
	

