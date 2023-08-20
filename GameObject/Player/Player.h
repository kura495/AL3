#pragma once
#include "BaseClass/BaseCharacter.h"
#include <numbers>
#include <cassert>
#include <imgui.h>
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Calc/Matrix.h"
#include "Calc/Vector.h"
class Player {
public:
	void Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm);
	void Updete();

	void Draw(const ViewProjection& viewProjection_);

	const WorldTransform& GetWorldTransform() { return worldTransformBase_; };
	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

private:
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	
	Model* model_ = nullptr;
	Model* modelBody_ = nullptr;
	Model* modelHead_ = nullptr;
	Model* modelL_arm_ = nullptr;
	Model* modelR_arm_ = nullptr;
	uint32_t textureHandle_ = 0u;
	//ローカル座標
	WorldTransform worldTransformBase_;
	//各パーツのローカル座標
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	//ゲームパッド
	XINPUT_STATE joyState;

	void SetParent(const WorldTransform* parent);
	void InitializeFloatingGimmick();
	void UpdateFloatingGimmick();
	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	int floatcycle_ = 120;
	float floatingAmplitude_ = 0.2f;
};
