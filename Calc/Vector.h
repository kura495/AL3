#pragma once
#include"Vector3.h"
#include<cmath>
#include<numbers>
#include<cassert>
#include <algorithm>
#include<vector>
Vector3 Add(const Vector3& m1, const Vector3& m2);
Vector3 Subtract(const Vector3& m1, const Vector3& m2);
Vector3 Normalize(const Vector3& v1);
/// <summary>
/// Lerpだと名前かぶりを起こすので注意
/// /summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 VectorLerp(const Vector3& v1, const Vector3& v2, float t);
Vector3 VectorSLerp(const Vector3& v1, const Vector3& v2, float t);
float dot(const Vector3& v1, const Vector3& v2);

Vector3 CatMull_Rom(const std::vector<Vector3>& controlPoints, const float& t);