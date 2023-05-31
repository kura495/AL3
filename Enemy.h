#pragma once
#include"Model.h"
#include "WorldTransform.h"
#include"WorldTransformEx.h"
#include"ImGuiManager.h"
#include"ImGuiSupport.h"
#include"EnemyBullet.h"
#include"MatrixCalc.h"
#include"VectorCalc.h"
#include<list>
#include"TimedCall.h"

class Enemy;

class PhaseState {
public:
	virtual void Update(Enemy* enemy, const Vector3& velocity) = 0;
};
class PhaseApproach : public PhaseState {
public:
	void Update(Enemy* enemy, const Vector3& velocity);
};
class PhaseLeave : public PhaseState {
public:
	void Update(Enemy* enemy, const Vector3& velocity);
};

class Enemy {
public:

	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection viewProjection);

	Vector3 GetTransform() { return worldTransform_.translation_; }
	//ステートパターンで使う移動用関数
	void WorldTransformAdd(const Vector3& velocity);
	void WorldTransformSubtract(const Vector3& velocity);
	void PhaseChange(PhaseState* newState);
	//フェーズごとのアップデート
	void ApproachUpdate();
	//フェーズごとの初期化
	void ApproachInitialize();
	void LeaveInitialize();

private:

	const float kEnemySpeed = -0.2f;
	const float kEnemySpeedY = 0.02f;
	Vector3 velocity_ = {0, kEnemySpeedY, kEnemySpeed};
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	WorldTransformEx worldTransformEx_;
	//玉の発射間隔
	std::list<EnemyBullet*> bullets_;
	//プライベート関数
	void Fire();
	std::list<TimedCall*> timedCalls_;
	
	
	PhaseState* state_;

};
