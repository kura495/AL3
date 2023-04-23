#pragma once
#include"Model.h"
#include "WorldTransform.h"
class Player {
public:

	void Initialize(Model*model,uint32_t textureHandle);

	void Updete();

	void Draw();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

};
