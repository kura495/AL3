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
//クラスの前方宣言
class Player;
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

	Vector3 GetWorldPosition();
	
	void SetPlayer(Player* player) { player_ = player; }

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
	//基本的な変数
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	//移動量
	const float kEnemySpeed = -0.2f;
	const float kEnemySpeedY = 0.02f;
	//移動量(Vector)
	Vector3 velocity_ = {0, kEnemySpeedY, kEnemySpeed};
	
	WorldTransformEx worldTransformEx_;
	
	//玉
	std::list<EnemyBullet*> bullets_;
	//玉の発射
	void Fire();
	//自キャラ(ここでは絶対にnullptr)(setterで)
	Player* player_ = nullptr;
	//ステートパターン
	
	std::list<TimedCall*> timedCalls_;

	PhaseState* state_;

};
