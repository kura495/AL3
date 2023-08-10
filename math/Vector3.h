#pragma once
/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;
	Vector3 operator*(const float& other) { 
		return Vector3(x*other,y*other,z*other);
	}
	Vector3 operator-(const Vector3& other) { 
		return Vector3(x-other.x,y-other.y,z-other.z);
	}
};