#pragma once
#include<Model.h>
#include<Vector3.h>
#include<WorldTransform.h>
#include"WorldTransformEx.h"
#include<cassert>

class EnemyBullet {
public:

	void Initialize(Model*model,const Vector3&position);
	void Update();
	void Draw(const ViewProjection& viewProjection);


private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_;
	WorldTransformEx* worldTransformEx;
	void UpdateMatrix();
};
