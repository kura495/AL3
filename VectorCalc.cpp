#include"VectorCalc.h"
Vector3 Add(const Vector3& m1, const Vector3& m2) {
	return Vector3{m1.x + m2.x, m1.y + m2.y, m1.z + m2.z};
}

Vector3 Subtract(const Vector3& m1, const Vector3& m2) {
	return Vector3{m1.x - m2.x, m1.y - m2.y, m1.z - m2.z};
}

