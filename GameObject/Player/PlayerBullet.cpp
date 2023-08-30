#include "PlayerBullet.h"
#include <cassert>

void PlayerBullet::Initialize(const Vector3& position, const Vector3& velocity) {
	// モデル読み込み
	model_ = Model::Create();
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("sample.png");
	// プレイヤーの向きを玉にも反映
	velocity_ = velocity;
	// ワールドトランスフォーム初期化
	worldTransform_.Initialize();
	// 初期値セット
	worldTransform_.translation_ = position;
	worldTransform_.translation_.y += 1;
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 0.5f;

	// 当たり判定とマスク設定
	SetRadius(Radius_);
	SetcollitionAttribute(kCollitionAttributePlayer);
	SetcollisionMask(~kCollitionAttributePlayer);
}

void PlayerBullet::Update() {

	// 玉の移動
	Move();

	worldTransform_.UpdateMatrix();

	// 時間経過でデスフラグを立てる
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}

void PlayerBullet::OnCollision() { isDead_ = true; }

Vector3 PlayerBullet::GetWorldPosition() {
	Vector3 worldPos;
	worldTransform_.UpdateMatrix();
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

// private関数
void PlayerBullet::Move() {
	// 加算
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
}