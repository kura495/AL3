#pragma once
#include"WorldTransform.h"
#include"MatrixCalc.h"
class WorldTransformEx {
public:
	WorldTransformEx();
	~WorldTransformEx();

	void UpdateMatrix(
	    WorldTransform &worldTransform, const Vector3& scale, const Vector3& rotate,
	    const Vector3& translate);

private:

};

