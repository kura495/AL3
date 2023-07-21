﻿#pragma once
#include<Vector3.h>
#include<Matrix4x4.h>
#include<cmath>
#include<cassert>
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
float det(const Matrix4x4& m);
Matrix4x4 Inverse(const Matrix4x4& m);
Vector3 Transformed(const Vector3& vector, const Matrix4x4& matrix);