#pragma once
#include <WorldTransform.h>
#include <Model.h>
#include <ViewProjection.h>
#include <numbers>

/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	// 移動速度
	static inline const float kWalkSpeed = 0.02f;
	// 最初の角度[度]
	static inline const float kWalkMotionAngleStart = 1.5f;
	// 最後の角度[度]
	static inline const float kWalkMotionAngleEnd = 1.0f;
	// アニメーションの周期となる時間[秒]
	static inline const float kWalkMotionTime = 1.0f;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// イージング関数
	/// </summary>
	float fLerp(float start, float end, float t) { return (1.0f - t) * start + t * end; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;

	// 速度
	Vector3 velocity_ = {};

	// 経過時間
	float walkTimer_ = 0.0f;
};
