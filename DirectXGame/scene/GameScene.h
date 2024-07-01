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

	// デバッグカメラ
	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;

	// skydome
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;

	// ブロック
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	Model* modelBlock_ = nullptr;

	// マップチップフィールド
	MapChipField* mapChipField_;

	// 自キャラ
	Player* player_ = nullptr;
	Model* modelPlayer_ = nullptr;

	// 敵キャラ
	//Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemies_;
	Model* modelEnemy_ = nullptr;

	// カメラコントローラ
	CameraController* cameraController_ = nullptr;
	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
};
