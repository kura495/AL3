#include "Enemy/State/PhaseApproach.h"
#include"Enemy/Enemy.h"
void PhaseApproach::Update(Enemy* enemy, const Vector3& velocity) {

	if (enemy->GetTransform().z < -30.0f) {
		enemy->PhaseChange(new PhaseLeave());
	}
	enemy->WorldTransformAdd(velocity);
}