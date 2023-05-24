#pragma once
#include"Model.h"
#include "WorldTransform.h"
#include"WorldTransformEx.h"
#include"ImGuiManager.h"
#include"ImGuiSupport.h"
#include"Phase.h"

class PhaseState;
class PhaseApproach;
class PhaseLeave;

class Enemy {
public:
	Enemy();
	~Enemy();

	void Initialize(Model* model);

	void Update();

	void StateUpdate();

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
class PhaseState {
public:
	PhaseState();
	~PhaseState();
	virtual void Update(Enemy* enemy) = 0;
};

class PhaseApproach : public PhaseState {
public:
	PhaseApproach();
	~PhaseApproach();
	void Update(Enemy* enemy);
};

class PhaseLeave : public PhaseState {
public:
	PhaseLeave();
	~PhaseLeave();
	void Update(Enemy* enemy);
};
