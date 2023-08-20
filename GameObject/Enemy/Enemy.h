#pragma once
#include "BaseClass/BaseCharacter.h"
#include "Calc/Matrix.h"
#include "Calc/Vector.h"
class Enemy : public BaseCharacter{
public:
	void Initialize(const std::vector<Model*>& models) override;
	void Update() override;
	void Draw(const ViewProjection& viewProjection) override;

private:
};

