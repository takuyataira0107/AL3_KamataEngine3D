#include "MathUtilityForText.h"

// 代入演算子オーバーロード
// Vector3の足算
Vector3& operator+=(Vector3& lhs, const Vector3& rhv) {
	lhs.x += rhv.x;
	lhs.y += rhv.y;
	lhs.z += rhv.z;
	return lhs;
}

// 2頂演算子オーバーロード
// Vector3の足算
const Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp += v2;
}

// 代入演算子オーバーロード
// Vector3の掛け算
Vector3& operator*=(Vector3& v, const float s) {
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return v;
}

// 2頂演算子オーバーロード
// Vector3の掛け算
const Vector3 operator*(const Vector3& v, const float s) {
	Vector3 temp(v);
	return temp *= s;
}