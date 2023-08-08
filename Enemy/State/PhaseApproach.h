#pragma once
#include "Enemy/State/Istate.h"
class PhaseApproach : public PhaseState {
public:
	void Update(Enemy* enemy, const Vector3& velocity);
};