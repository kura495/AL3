#include"PhaseLeave.h"
#include"GameObject/Enemy/Enemy.h"

void PhaseLeave::Update(Enemy* enemy, const Vector3& velocity) {
	if (enemy->GetTransform().z > 30.0f) {
		enemy->ApproachInitialize();
		enemy->PhaseChange(new PhaseApproach());
	}
	enemy->WorldTransformSubtract(velocity);
}