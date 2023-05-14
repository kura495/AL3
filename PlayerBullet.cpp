#include"PlayerBullet.h"
#include<cassert>
PlayerBullet::PlayerBullet() {}

PlayerBullet::~PlayerBullet() {}

void PlayerBullet::Initialize(Model* model, const Vector3& position) { 
	//modelチェック
	assert(model);
	//モデル読み込み
	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("sample.png");
	//ワールドトランスフォーム初期化
	worldTransform_.Initialize();
	//初期値セット
	worldTransform_.translation_ = position;
}

void PlayerBullet::Update() { 
	//玉の移動
	Move();

	worldTransformEx_.UpdateMatrix(
	    worldTransform_, worldTransform_.scale_, worldTransform_.rotation_,
	    worldTransform_.translation_);
}

void PlayerBullet::Draw(const ViewProjection viewProjection_) { 
	model_->Draw(worldTransform_,viewProjection_,textureHandle_);
}

//private関数
void PlayerBullet::Move() {
	// 玉の移動ベクトル
	Vector3 move = {0, 0, 0};
	// 玉の移動の速さ
	const float BulletSpeed = 0.2f;
	move.y += BulletSpeed;
	// 移動制限
	/*const float kMoveLimitX = 30;
	const float kMoveLimitY = 15;
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);*/
	// 加算
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

}
