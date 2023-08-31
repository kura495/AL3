#include "Enemy.h"

void Enemy::Initialize(const std::vector<Model*>& models){
	//基底クラスの初期化
	BaseCharacter::Initialize(models);
	SetRadius(Radius_);
	SetcollitionAttribute(kCollitionAttributeEnemy);
	SetcollisionMask(~kCollitionAttributeEnemy &  ~kCollitionAttributePlayer);
}

void Enemy::Update() {
	// 速さ
	const float kSpeed = 0.3f;
	Vector3 velocity{0.0f, 0.0f, kSpeed};

	// 移動ベクトルをカメラの角度だけ回転
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 移動量
	//TODO:当たり判定テストのためコメントアウト中 コメントアウトを消す　
	//worldTransform_.translation_ = Add(worldTransform_.translation_, velocity);
	
	// 自機のY軸周り角度(θy)
	worldTransform_.rotation_.y += 0.03f;

	// 基底クラスの更新処理
	BaseCharacter::Update();
}

void Enemy::Draw(const ViewProjection& viewProjection){
	BaseCharacter::Draw(viewProjection); 
}

void Enemy::OnCollision() { 
		IsAlive_ = false;
}

void Enemy::SetPosition(const Vector3& position){ 
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;
	worldTransform_.UpdateMatrix();
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
