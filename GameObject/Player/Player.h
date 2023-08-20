#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Calc/Matrix.h"
#include "Calc/Vector.h"
class Player {
public:

	void Initialize(Model*model);

	void Updete();

	void Draw(const ViewProjection& viewProjection_);

	const WorldTransform& GetWorldTransform() { return worldTransform_; };
	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

private:
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	//ローカル座標
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	//ゲームパッド
	XINPUT_STATE joyState;

};
