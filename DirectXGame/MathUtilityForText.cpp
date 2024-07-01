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

// AABBの衝突判定
bool isCollisionAABB(const AABB& a, const AABB& b) {
	if ((a.min.x <= b.max.x && a.max.x >= b.min.x) && // x軸
	    (a.min.y <= b.max.y && a.max.y >= b.min.y) && // y軸
	    (a.min.z <= b.max.z && a.max.z >= b.min.z)) { // z軸
		return true;
	}

	return false;
}