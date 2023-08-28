#pragma once
#include <numbers>
#include <cassert>
#include <imgui.h>
#include <optional>
#include "BaseClass/BaseCharacter.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Calc/Matrix.h"
#include "Calc/Vector.h"
#include "Utility/CollisionConfig.h"
#include "PlayerBullet.h"

enum modelNumber {
	kModelIndexBody,
	kModelIndexHead,
	kModelIndexL_arm,
	kModelIndexR_arm,
	kModelIndexWeapon,
};
enum class Behavior {
	kRoot,//通常
	kAttack,//攻撃
};
class Player : public BaseCharacter{
public:
	Player(){};
	~Player() {
		for (PlayerBullet* bullet_ : bullets_) {
			delete bullet_;
		}
	};
	void Initialize(const std::vector<Model*>& models) override;
	void Update() override;

	void Draw(const ViewProjection& viewProjection_) override;

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	void OnCollision() override;
	Vector3 GetWorldPosition() override;
	

private:
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	//各パーツのローカル座標
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	WorldTransform worldTransformWeapon_;
	//ゲームパッド
	XINPUT_STATE joyState;

	std::list<PlayerBullet*> bullets_;
	void Shot();

	Vector3 move;

	// 浮遊ギミック
	void SetParent(const WorldTransform* parent);
	void InitializeFloatingGimmick();
	void UpdateFloatingGimmick();
	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	int floatcycle_ = 120;
	float floatingAmplitude_ = 0.2f;

	//RootBehavior
	//次のふるまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	Behavior behavior_ = Behavior::kRoot;
	//通常
	void BehaviorRootInitialize();
	void BehaviorRootUpdate();
	//攻撃
	void BehaviorAttackInitialize();
	void BehaviorAttackUpdate();
	int attackAnimationFrame;
};
