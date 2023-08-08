#pragma once
#include<WorldTransform.h>
#include"WorldTransformEx.h"
#include"Affine.h"
class Enemy;
class PhaseState {
public:
	virtual void Update(Enemy* enemy, const Vector3& velocity) = 0;
};
