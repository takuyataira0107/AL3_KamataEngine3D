#pragma once
#include "Model.h"
#include "WorldTransform.h"

class MapChipField;

enum class LRDirection {
	kRight,
	kLeft,
};

class Player {
public:
	static inline const float kAcceleration = 0.04f;
	static inline const float kAttenuation = 0.02f;
	static inline const float kLimitRunSpeed = 0.1f;
	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;
	// 重力加速度（下方向）
	static inline const float kGravityAcceleration = 0.05f;
	// 最大落下速度（下方向）
	static inline const float kLimitFallSpeed = 0.2f;
	// ジャンプ初速（上方向）
	static inline const float kJumpAcceleration = 0.5f;
	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	static inline const float kBlank = 0.02f;
	// 着地時の速度減衰率
	static inline const float kAttenuationLanding = 0.0f;
	static inline const float kCollisionExtension = 0.05f;
	// 着地時の速度減衰率
	static inline const float kAttenuationWall = 0.3f;

	struct CollisionMapInfo {
		bool ceilingCollision = false;
		bool landing = false;
		bool hitWall = false;
		Vector3 move;
	};

	// 角
	enum Corner {
		kRightBottom,   // 右下
		kLeftBottom,    // 左下
		kRightTop,      // 右上
		kLeftTop,       // 左上

		kNumCorner      // 要素数
	};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);


	/// <summary>
	/// 移動入力
	/// </summary>
	void InputMove();

	/// <summary>
	/// 角の座標計算
	/// </summary>
	Vector3 CornerPosition(const Vector3& center, Corner corner);

	/// <summary>
	/// マップ衝突判定
	/// </summary>
	void MapCollision(CollisionMapInfo& info);
	void MapCollisionTop(CollisionMapInfo& info);
	void MapCollisionBottom(CollisionMapInfo& info);
	void MapCollisionRight(CollisionMapInfo& info);
	void MapCollisionLeft(CollisionMapInfo& info);

	/// <summary>
	/// 判定結果を判定して移動させる
	/// </summary>
	void MoveReflection(const CollisionMapInfo& info);

	/// <summary>
	/// 天井に接触している場合の処理
	/// </summary>
	void HitCeilingCase(const CollisionMapInfo& info);

	/// <summary>
	/// 接地状態の切り替え処理
	/// </summary>
	void SwitchingOnGround(const CollisionMapInfo& info);

	/// <summary>
	/// 壁に接触している場合の処理
	/// </summary>
	void HitWallCase(const CollisionMapInfo& info);

	/// <summary>
	/// 旋回制御
	/// </summary>
	void TurningControl();

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
	float easeInOutCubic(float x) { return x < 0.5 ? 4 * x * x * x : 1 - powf(-2 * x + 2, 3) / 2; }

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const Vector3& GetVelocity() const { return velocity_; }
	const Vector3& GetPlayerPosition() const { return worldTransform_.translation_; }
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// ビュープロジェクション
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

	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;
};