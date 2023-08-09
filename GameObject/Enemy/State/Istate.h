#pragma once
#include"Vector3.h"
class Enemy;
class PhaseState {
public:
	virtual void Update(Enemy* enemy, const Vector3& velocity) = 0;
	virtual  ~PhaseState();
};
inline PhaseState::~PhaseState() {}