#pragma once
#include"Istate.h"
class PhaseLeave : public PhaseState {
public:
	void Update(Enemy* enemy, const Vector3& velocity);
};