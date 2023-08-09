#include"Calc/Vector.h"
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	return Vector3{v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	return Vector3{v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

Vector3 Normalize(const Vector3& v1) { 
	Vector3 Result = v1;
	float length = sqrt(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
	assert(length != 0);
	Result.x /= length;
	Result.y /= length;
	Result.z /= length;
	return Result;
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
	float h = dot(v1, v2);
	float Costheta = std::acos((h * (float)std::numbers::pi) / 180);
	float Sintheta = std::sin(Costheta);
	float Pstert = std::sin((1 - t) * Costheta) / Sintheta;
	float Pend = std::sin(t * Costheta) / Sintheta;
	result.x = (Pstert * v1.x + Pend * v2.x);
	result.y = (Pstert * v1.y + Pend * v2.y);
	result.z = (Pstert * v1.z + Pend * v2.z);

	return result;
}

float dot(const Vector3& v1, const Vector3& v2) {
	float result = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
	return result;
}

Vector3 CatMull_Rom(const std::vector<Vector3>& controlPoints, const float& t) {
	int n = (int)controlPoints.size();
	int segment = static_cast<int>(t * (n - 1));
	float tSegment = t * (n - 1) - segment;

	Vector3 p0 = controlPoints[segment > 0 ? segment - 1 : 0];
	Vector3 p1 = controlPoints[segment];
	Vector3 p2 = controlPoints[segment < n - 1 ? segment + 1 : n - 1];
	Vector3 p3 = controlPoints[segment < n - 2 ? segment + 2 : n - 1];

	Vector3 interpolatedPoint;
	interpolatedPoint.x =
	    0.5f * ((2.0f * p1.x) + (-p0.x + p2.x) * tSegment +
	            (2.0f * p0.x - 5.0f * p1.x + 4.0f * p2.x - p3.x) * (tSegment * tSegment) +
	            (-p0.x + 3.0f * p1.x - 3.0f * p2.x + p3.x) * (tSegment * tSegment * tSegment));
	interpolatedPoint.y =
	    0.5f * ((2.0f * p1.y) + (-p0.y + p2.y) * tSegment +
	            (2.0f * p0.y - 5.0f * p1.y + 4.0f * p2.y - p3.y) * (tSegment * tSegment) +
	            (-p0.y + 3.0f * p1.y - 3.0f * p2.y + p3.y) * (tSegment * tSegment * tSegment));
	interpolatedPoint.z =
	    0.5f * ((2.0f * p1.z) + (-p0.z + p2.z) * tSegment +
	            (2.0f * p0.z - 5.0f * p1.z + 4.0f * p2.z - p3.z) * (tSegment * tSegment) +
	            (-p0.z + 3.0f * p1.z - 3.0f * p2.z + p3.z) * (tSegment * tSegment * tSegment));

	return interpolatedPoint;
}
