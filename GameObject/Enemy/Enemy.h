#pragma once
#include "BaseClass/BaseCharacter.h"
#include "Calc/Matrix.h"
#include "Calc/Vector.h"
#include "Utility/CollisionConfig.h"

class Enemy : public BaseCharacter{
public:
	void Initialize(const std::vector<Model*>& models) override;
	void Update() override;
	void Draw(const ViewProjection& viewProjection) override;
	void OnCollision() override;
	Vector3 GetWorldPosition() override;
	bool GetIsAlive() const { return IsAlive_; };

private:
	bool IsAlive_ = TRUE;
	float Radius_ = 1.0f;
};

