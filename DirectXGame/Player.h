#pragma once
#include "Model.h"
#include "WorldTransform.h"


enum class LRDirection {
	kRight,
	kLeft,
};

class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
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
	float Lerp(float start, float end, float t) { return (1.0f - t) * start + t * end; }
	float easeInOutCubic(float x) { return x < 0.5 ? 4 * x * x * x : 1 - powf(-2 * x + 2, 3) / 2; }

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const Vector3& GetVelocity() const { return velocity_; }
	const Vector3& GetPlayerPosition() const { return worldTransform_.translation_; }

private:
	static inline const float kAcceleration = 0.08f;
	static inline const float kAttenuation = 0.02f;
	static inline const float kLimitRunSpeed = 0.2f;
	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;
	// 重力加速度（下方向）
	static inline const float kGravityAcceleration = 0.05f;
	// 最大落下速度（下方向）
	static inline const float kLimitFallSpeed = 0.2f;
	// ジャンプ初速（上方向）
	static inline const float kJumpAcceleration = 0.5f;


	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	//uint32_t textureHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;

	// 速度
	Vector3 velocity_ = {};

	LRDirection lrDirection_ = LRDirection::kRight;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;


	// 接地状態フラグ
	bool onGround_ = true;
};