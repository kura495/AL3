#pragma once
#include"Model.h"
#include "WorldTransform.h"
#include"WorldTransformEx.h"

class Enemy {
public:
	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection viewProjection);
	
private:

	enum class Phase {
		approach,
		Leave,
	};

	Phase phase_ = Phase::approach;
	
	const float kEnemySpeed = -0.2f;
	const float kEnemySpeedY = 0.02f;
	Vector3 velocity_ = {0, kEnemySpeedY, kEnemySpeed};
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	WorldTransformEx worldTransformEx_;
	//プライベート関数
	void Move();
	void PhaseApproach();
	void PhaseLeave();
};

