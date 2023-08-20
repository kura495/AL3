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
enum modelNumber {
	kModelIndexBody,
	kModelIndexHead,
	kModelIndexL_arm,
	kModelIndexR_arm,
};

class Player :public BaseCharacter{
public:
	void Initialize(const std::vector<Model*>& models)override;
	void Updete();

	void Draw(const ViewProjection& viewProjection_)override;

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

private:
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

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
