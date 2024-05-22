#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"


//行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

//行列の減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

//行列の積
Matrix4x4 Multiply(const Matrix4x4& matrix1, const Matrix4x4& matrix2);

//逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
