#include "Enemy/State/PhaseLeave.h"
#include"Enemy/Enemy.h"
void PhaseLeave::Update(Enemy* enemy, const Vector3& velocity) {
	if (enemy->GetTransform().z > 30.0f) {
		enemy->PhaseChange(new PhaseApproach());
	}
	enemy->WorldTransformSubtract(velocity);
}