#pragma once
#include"Model.h"
#include "WorldTransform.h"
#include"Input.h"
#include"Calc/Matrix.h"
#include"ImGuiManager.h"
#include"Utility/ImGuiSupport.h"
#include"Utility/Collider.h"
#include"Utility/CollisionConfig.h"
#include"PlayerBullet.h"
#include"Sprite.h"
#include<list>
class Player : public Collider {
public:
	Player();
	~Player();

	void Initialize(Model* model, uint32_t textureHandle, Vector3 Position);

	void Updete(const ViewProjection& viewProjection);

	void Draw(const ViewProjection viewProjection_);
	void DrawUI();
	Vector3 GetWorldPosition();
	void OnCollision();
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	void SetParent(const WorldTransform* parent);

private:
	
	void Move();
	void Rotate();
	void Attack();
	
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	std::list<PlayerBullet*> bullets_;
	

	WorldTransform worldTransform3DReticle_;
	void SetReticle();

	Model* ReticleModel;
	Sprite* sprite2DReticle_ = nullptr;
	HWND hwnd = nullptr;
	void Dreticle3DWorldToDreticle2DScreen(const ViewProjection& viewProjection);
	void GetMauseCursur();
};
