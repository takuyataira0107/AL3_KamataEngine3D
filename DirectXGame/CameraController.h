#pragma once
#include <ViewProjection.h>


// 前方宣言
class Player;

/// <summary>
/// カメラコントローラ
/// </summary>
class CameraController {
public:
	// 矩形
	struct Rect {
		float left = 0.0f;   // 左端
		float right = 1.0f;  // 右端
		float bottom = 0.0f; // 下端
		float top = 1.0f;    // 上端
	};

	// 座標補間割合
	static inline const float kInterpolationRate = 0.2f;
	// 速度掛け率
	static inline const float kVelocityBias = 30.0f;
	// 追従対象の各方向へのカメラ移動範囲
	static inline const Rect margin = {-30.0f, 30.0f, 30.0f, 30.0f};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void Reset();

	// 線形補間
	float Lerp(float start, float end, float t) { return (1.0f - t) * start + t * end; }

	void SetTarget(Player* target) { target_ = target; }
	void SetMovableArea_(Rect area) { movableArea_ = area; }
	const ViewProjection& GetViewProjection() const { return viewProjection_; }

private:
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 追従対象
	Player* target_ = nullptr;
	// 追従対象とカメラの座標の差（オフセット）
	Vector3 targetOffset_ = {0, 0, -15.0f};
	
	// カメラ移動制限
	Rect movableArea_ = {0, 100, 0, 100};

	// カメラの目標座標
	Vector3 goalCoord_;

};
