#pragma once
#include<Model.h>
#include<Vector3.h>
#include<WorldTransform.h>
#include"Calc/Vector.h"
#include<cassert>
#include"Utility/Collider.h"
class Player;
class EnemyBullet : public Collider {
public:

	void Initialize(Model* model, const Vector3& position, const Vector3& velosity);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	bool Isdead() const { return isDead_; }
	void SetPlayer(Player* player) { player_ = player; }
	void OnCollision();
	Vector3 GetWorldPosition();

private:

	void Move();
	void Homing();

	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_;
	Vector3 velocity_;
	//玉の寿命
	static const int32_t kLifeTime = 60 * 3;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
	// 玉の速さ
	const float kBulletSpeed = 0.5f;
	Vector3 toPlayer;
	// ここでは絶対nullptr setterを利用
	Player* player_ = nullptr;
};
