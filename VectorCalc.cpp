#include"VectorCalc.h"
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	return Vector3{v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	return Vector3{v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

Vector3 Normalize(const Vector3& v1) { 
	float Root = sqrt(v1.x * v1.x + v1.y * v1.y+v1.z+v1.z);
	assert(Root != 0);
	return Vector3(v1.x/Root,v1.y/Root,v1.z/Root);
}

