#pragma once
#include"WorldTransform.h"
#include"Model.h"
class Skydome {
public:
	void Initialize(Model* model);
	void Update();
	void Draw(const ViewProjection viewProjection_);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
};

