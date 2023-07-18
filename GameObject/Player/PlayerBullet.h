#pragma once
#include"Model.h"
#include "WorldTransform.h"
#include"Calc/Vector.h"
#include"Utility/Collider.h"
#include"Utility/CollisionConfig.h"
class PlayerBullet : public Collider {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, const Vector3& position,const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void Draw(const ViewProjection viewProjection);

	bool IsDead() const { return isDead_; };
	void OnCollision();
	Vector3 GetWorldPosition();

private:

	void Move();
	
	Vector3 velocity_;
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	//玉の寿命
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};

