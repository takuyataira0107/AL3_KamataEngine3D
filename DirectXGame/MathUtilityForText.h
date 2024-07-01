#pragma once
#include <Vector3.h>
class MathUtilityForText {};

struct AABB {
	Vector3 min;
	Vector3 max;
};

// 代入演算子オーバーロード
// Vector3の足算
Vector3& operator+=(Vector3& lhs, const Vector3& rhv);

// 2頂演算子オーバーロード
// Vector3の足算
const Vector3 operator+(const Vector3& v1, const Vector3& v2);

// 代入演算子オーバーロード
// Vector3の掛け算
Vector3& operator*=(Vector3& v, const float s);

// 2頂演算子オーバーロード
// Vector3の掛け算
const Vector3 operator*(const Vector3& v, const float s);


// AABBの衝突判定
bool isCollisionAABB(const AABB& a, const AABB& b);