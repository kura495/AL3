#pragma once
#include"Model.h"
#include "WorldTransform.h"
#include"ImGuiManager.h"
#include"Utility/ImGuiSupport.h"
#include"Utility/Collider.h"
#include"EnemyBullet.h"
#include"Calc/Matrix.h"
#include"Calc/Vector.h"
#include<list>
#include"Utility/TimedCall.h"
#include"Utility/CollisionConfig.h"
//クラスの前方宣言
class Player;
class Enemy;
class GameScene;

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

class Enemy : public Collider {
public:

	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection viewProjection);
	//ワールド座標を渡す
	Vector3 GetWorldPosition();
	/// <summary>
	/// trueなら死亡
	/// </summary>
	/// <returns></returns>
	bool Isdead() const { return isDead_; }
	void SetPlayer(Player* player) { player_ = player; }
	//ゲームシーンを渡す
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	void OnCollision();
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
	// デスフラグ
	bool isDead_ = false;
	//玉
	std::list<TimedCall*> timedCalls_;
	//玉の発射
	void Fire();
	//自キャラ(ここでは絶対にnullptr)(setterで)
	Player* player_ = nullptr;
	//ステートパターン
	PhaseState* state_;
	//ゲームシーン
	GameScene* gameScene_ = nullptr;
};
