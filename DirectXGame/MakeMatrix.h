#pragma once
#include "MatrixCalc.h"
#include <assert.h>
#include <cmath>

	// 平行移動行列の作成関数 
	Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	// 拡大縮小行列の作成関数
	Matrix4x4 MakeScaleMatrix(const Vector3& scale);

	// 回転行列の作成関数
	Matrix4x4 MakeRotateXMatrix(float radian);
	Matrix4x4 MakeRotateYMatrix(float radian);
	Matrix4x4 MakeRotateZMatrix(float radian);

	// affine行列の作成関数
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	// 3次元ベクトルを同次座標として変更する
	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

	// 転置行列の作成関数
	Matrix4x4 Transpose(const Matrix4x4& m);

	// 単位行列の作成関数
	Matrix4x4 MakeIdentity4x4();

	// 正射影行列の作成関数
	Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

	// 透視投影行列の作成関数
	Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

	// ビューポート変換行列の作成関数
    Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);
