#pragma once
#include"Model.h"
#include "WorldTransform.h"
#include"WorldTransformEx.h"
#include"ImGuiManager.h"
#include"ImGuiSupport.h"
#include"Phase.h"

class Enemy;

class PhaseState {
public:
	virtual Vector3 Update(Enemy* enemy, Vector3* velocity) = 0;
};

class PhaseApproach : public PhaseState {
public:
	Vector3 Update(Enemy* enemy, Vector3* velocity);
};

class PhaseLeave : public PhaseState {
public:
	Vector3 Update(Enemy* enemy, Vector3* velocity);
};


class Enemy {
public:

	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection viewProjection);

	Vector3 GetTransform() { return worldTransform_.translation_; }

	void PhaseChange(PhaseState* newState);
private:

	const float kEnemySpeed = -0.2f;
	const float kEnemySpeedY = 0.02f;
	Vector3 velocity_ = {0, kEnemySpeedY, kEnemySpeed};
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	WorldTransformEx worldTransformEx_;
	//プライベート関数

	PhaseState* state_;

};
