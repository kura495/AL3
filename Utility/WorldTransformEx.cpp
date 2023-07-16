#include<WorldTransform.h>
#include"Calc/Matrix.h"
void WorldTransform::UpdateMatrix() {
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
	TransferMatrix();
}


