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

	void Draw(ViewProjection viewProjection_);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	//ゲームパッド
	XINPUT_STATE joyState;
};
