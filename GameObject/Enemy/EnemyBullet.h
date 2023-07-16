#pragma once
#include<Model.h>
#include<Vector3.h>
#include<WorldTransform.h>
#include"WorldTransformEx.h"
#include"VectorCalc.h"
#include<cassert>

class EnemyBullet {
public:

	void Initialize(Model* model, const Vector3& position, const Vector3& velosity);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	bool Isdead() const { return isDead_; }

private:

	void Move();

	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_;
	Vector3 velocity_;
	WorldTransformEx worldTransformEx;
	//玉の寿命
	static const int32_t kLifeTime = 60 * 3;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
};
