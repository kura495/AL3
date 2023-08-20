#include"Calc/Vector.h"
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	return Vector3{v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	return Vector3{v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

Vector3 Normalize(const Vector3& v1) { 
	Vector3 result{};
	float length = Length(v1);
	if (length != 0.0f) {
		result.x = v1.x / length;
		result.y = v1.y / length;
		result.z = v1.z / length;
	}
	return result;
}
Vector3 VectorLerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result;
	if (t < 0) {
		t = (float)std::min(0, 1);
	} else if (t > 1) {
		t = (float)std::max(0, 1);
	}

	result.x = v1.x + t * (v2.x - v1.x);
	result.y = v1.y + t * (v2.y - v1.y);
	result.z = v1.z + t * (v2.z - v1.z);

	return result;
}
Vector3 VectorSLerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result;
	float h = Dot(v1, v2);
	float Costheta = std::acos((h * (float)std::numbers::pi) / 180);
	float Sintheta = std::sin(Costheta);
	float Pstert = std::sin((1 - t) * Costheta) / Sintheta;
	float Pend = std::sin(t * Costheta) / Sintheta;
	result.x = (Pstert * v1.x + Pend * v2.x);
	result.y = (Pstert * v1.y + Pend * v2.y);
	result.z = (Pstert * v1.z + Pend * v2.z);

	return result;
}

float Dot(const Vector3& v1, const Vector3& v2) {
	float result = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
	return result;
}
float Length(const Vector3& v) {
	float result{};
	result = sqrt(Dot(v, v));
	return result;
}