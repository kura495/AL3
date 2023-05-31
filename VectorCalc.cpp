#include"VectorCalc.h"
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	return Vector3{v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	return Vector3{v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

Vector3 Normalize(const Vector3& v1) { 
	float length = sqrt(v1.x * v1.x + v1.y * v1.y+v1.z+v1.z);
	assert(length != 0);
	return Vector3(v1.x/length,v1.y/length,v1.z/length);
}

