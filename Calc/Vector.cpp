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

//キャットムルロム曲線
Vector3 CatMull_Rom(const std::vector<Vector3>& controlPoints, const float& t) {
	//要素数を取得する
	//size関数は要素数を返す　例　std::vector<int>sanple = {1, 2, 3, } の時 sanple.size() = 3
	int elements = static_cast<int> (controlPoints.size());
	//tの値x(要素数-1) 0からスタートにしたいので、-1する
	//少数点以下を切り捨てるためにint型にする
	//線分の位置を特定する
	int segment = static_cast<int> (t * (elements - 1));
	//小数点以下を取り出すためにfloat型segment-int型segmentをする
	//線分の位置からtを0~1の範囲に収める
	float tSegment = t * (elements - 1) - segment;
	//始点
	//線分の位置が1以上なら線分の現在地からひとつ前を代入　線分の位置が0以下なら0を代入
	Vector3 p0;
	if (segment > 0) {
		p0 = controlPoints[segment - 1];
	} else {
		p0 = controlPoints[0];
	}
	//始点から伸びるベクトル
	//要素数-1より線分が大きくなったら、制御点から飛び出てしまうので要素数-1を代入
	Vector3 v0;
	if (segment < elements - 1) {
		v0 = controlPoints[segment + 1];
	} else {
		v0 = controlPoints[elements - 1];
	}
	//始点の一つ先の制御点
	Vector3 p1 = controlPoints[segment];
	//始点の一つ先の制御点から伸びるベクトル
	//要素数-2より線分が大きくなったら、制御点から飛び出てしまうので要素数-1を代入
	Vector3 v1;
	if (segment < elements - 2) {
		v1 = controlPoints[segment + 2];
	} else {
		v1 = controlPoints[elements - 1];
	}
	Vector3 result;
	//エルミート曲線の式に、全ての制御点を通るように変更を加えた式
	result.x =0.5f*
		(((-p0.x + 3.0f * p1.x - 3.0f * v0.x + v1.x) * (tSegment * tSegment * tSegment)) +
		((2.0f * p0.x - 5.0f * p1.x + 4.0f * v0.x - v1.x) * (tSegment * tSegment)) +
	    ((2.0f * p1.x) + (-p0.x + v0.x) * tSegment));
	result.y =0.5f * 
	    (((-p0.y + 3.0f * p1.y - 3.0f * v0.y + v1.y) * (tSegment * tSegment * tSegment)) +
	    ((2.0f * p0.y - 5.0f * p1.y + 4.0f * v0.y - v1.y) * (tSegment * tSegment)) +
		((2.0f * p1.y) + (-p0.y + v0.y) * tSegment));
	result.z =0.5f * 
	    (((-p0.z + 3.0f * p1.z - 3.0f * v0.z + v1.z) * (tSegment * tSegment * tSegment)) +
	    ((2.0f * p0.z - 5.0f * p1.z + 4.0f * v0.z - v1.z) * (tSegment * tSegment)) +
		((2.0f * p1.z) + (-p0.z + v0.z) * tSegment));
	return result;
}
//エルミート曲線
Vector3 Hermite(const Vector3& p0, const Vector3& v0, const Vector3& p1, const Vector3& v1,const float& t) {
	Vector3 result;
	// 2点p0とp1を通り,p0における接線がv0,p1における接線がv1となる3次曲線
	// p(t) = a(t*t*t) + b(t*t) + ct + d
	// 微分して傾きを求める式にする
	// p'(t) = 3a(t*t) + 2bt + c
	// t = 0 の時
	// p(0) = d = p0
	// p'(0) = c = v0
	//　① つまり　d = p0 で、　c = v0
	// t = 1 の時
	// p(1) = a + b + c + d = p1 //式1
	// p'(1) = 3a + 2b + c = v1 //式2
	// 連立方程式 式1と式2を使う aを消してbを求める
	// 3p1 - v1 = b + 2c + 3d　①をつかって
	// b = 3p1 - 3p0 - v1 -2v0 // 2c と 3dを移動させて、b = の形にしている
	// b = 3(p1 - p0) -v1 -2v0
	// 連立方程式 式1と式2を使う bを消してaを求める
	// v1 - 2p1 = a - c -2d ①を使って
	// a = v1 - 2p1 + v0 + 2p0 // c と　-2dを移動させて、a = の形にしている
	// a = -2(p1 + -p0) + v1 +v0
	// つまり、
	//p(t) = ( -2(p1 - p0) + v1 + v0)* t * t * t + 
	//( 3(p1 - p0) -v1 -2v0) * t * t + 
	//v0 * t +
	//p0
	//になる

	result.x = (2.0f * p0.x + -2.0f *p1.x + v1.x + v0.x) * (t * t * t) +
	           (-3.0f * p0.x + 3.0f * p1.x + -2.0f * v0.x - v1.x) * (t * t) + 
				v0.x * t +
				p0.x;

	/*result.x = (-p0.x + 3.0f * p1.x - 3.0f * v0.x + v1.x) * (t * t * t)) +
	            (2.0f * p0.x - 5.0f * p1.x + 4.0f * v0.x - v1.x) * (t * t) +
				(-p0.x + v0.x) * t +
				0.5f * ((2.0f * p1.x);*/
	result.y = (2.0f * p0.y + -2.0f * p1.y + v1.y + v0.y) * (t * t * t) +
	           (-3.0f * p0.y + 3.0f * p1.y + -2.0f * v0.y - v1.y) * (t * t) + v0.y * t + p0.y;
	result.z = (2.0f * p0.z + -2.0f * p1.z + v1.z + v0.z) * (t * t * t) +
	           (-3.0f * p0.z + 3.0f * p1.z + -2.0f * v0.z - v1.z) * (t * t) + v0.z * t + p0.z;

	return result;
}
