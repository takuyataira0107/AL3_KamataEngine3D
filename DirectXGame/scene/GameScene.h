#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <vector>
#include "DebugCamera.h"

#include "Player.h"
#include "Skydome.h"
#include "MapChipField.h"
#include "CameraController.h"
#include "Enemy.h"
#include "DeathParticles.h"

// ゲームのフェーズ（型）
enum class Phase {
	kPlay,     // ゲームプレイ
	kDeath,    // デス演出
//	kNext,	   // 次のステージ
	kGoal,    // ゲームクリア
};

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ブロック生成
	/// </summary>
	void GenerateBlocks();

	// 全ての当たり判定を行う
	void CheckAllCollisions();

	/// <summary>
	/// フェーズの切り替え
	/// </summary>
	void ChangePhase();

	// デスフラグの getter
	bool IsFinished() const { return finished_; }

	// ゴールフラグの getter
	bool IsGoalFinished() const { return goalFinished_; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// ゲームの現在のフェーズ
	Phase phase_;

	// デバッグカメラ
	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;

	// 天球
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;

	// ブロック
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	Model* modelBlock_ = nullptr;

	// ゴールブロック
	std::vector<std::vector<WorldTransform*>> worldTransformGoalBlocks_;
	Model* modelGoal_ = nullptr;

	// マップチップフィールド
	MapChipField* mapChipField_;

	// 自キャラ
	Player* player_ = nullptr;
	Model* modelPlayer_ = nullptr;

	// パーティクル
	DeathParticles* deathParticles_ = nullptr;
	Model* modelDeathParticles_ = nullptr;

	// 敵キャラ
	std::list<Enemy*> enemies_;
	Model* modelEnemy_ = nullptr;

	// カメラコントローラ
	CameraController* cameraController_ = nullptr;
	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};

	// 終了フラグ
	bool finished_ = false;

	// ゴール終了フラグ
	bool goalFinished_ = false;
};
