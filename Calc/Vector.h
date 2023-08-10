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
/// <summary>
/// CatMull-Rom曲線は、Hermite曲線を連続で繋げて、その間の制御点を全て通るようにしたもの
/// </summary>
/// <param name="controlPoints">std::Vector Vector3型 </param>
/// <param name="t">　パラメータ　媒介変数</param>
/// <returns></returns>
Vector3 CatMull_Rom(const std::vector<Vector3>& controlPoints, const float& t);
/// <summary>
/// Hermite曲線とは、始点と終点、始点ベクトルと終点ベクトルを使って作る曲線
/// Ferguson曲線とも言う
/// </summary>
///<param name="p0">始点</param>
///<param name="v0">始点から伸びるベクトル</param>
///<param name="p1">終点</param>
///<param name="v1">終点から伸びるベクトル</param>
///<param name="t">　パラメータ　媒介変数</param>
/// <returns></returns>
Vector3 Hermite(const Vector3& p0,const Vector3& v0, const Vector3& p1,  const Vector3& v1, const float& t);