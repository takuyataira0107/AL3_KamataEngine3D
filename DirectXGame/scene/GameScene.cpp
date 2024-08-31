#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>


GameScene::GameScene() {}

GameScene::~GameScene() {
	// 解放処理
	delete modelBlock_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
	delete debugCamera_;
	delete skydome_;
	delete modelSkydome_;
	delete mapChipField_;
	delete player_;
	delete modelDeathParticles_;
	delete modelPlayer_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	enemies_.clear();
	delete modelEnemy_;
	delete cameraController_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	
	// ゲームプレイフェーズから開始
	phase_ = Phase::kPlay;

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 天球を生成
	skydome_ = new Skydome();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// 天球を初期化
	skydome_->Initialize(modelSkydome_, &viewProjection_);

	// ブロックの生成
	modelBlock_ = Model::CreateFromOBJ("block", true);

	// マップチップの生成
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	// マップチップの初期化
	GenerateBlocks();

	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(3, 18);
	// 自キャラの生成
	player_ = new Player();
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	// 自キャラの初期化
	player_->Initialize(modelPlayer_, &viewProjection_, playerPosition);
	player_->SetMapChipField(mapChipField_);

	// 仮の生成処理。後で消す。
	deathParticles_ = new DeathParticles;
	modelDeathParticles_ = Model::CreateFromOBJ("deathParticle", true);
	
	// 敵キャラの生成
	modelEnemy_ = Model::CreateFromOBJ("enemy", true);
	for (int32_t i = 0; i < 3; ++i) {
		Enemy* newEnemy = new Enemy();
		// 敵キャラの座標
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(16 + 2 * i, 18);
		// 敵キャラの初期化
		newEnemy->Initialize(modelEnemy_, &viewProjection_, enemyPosition);

		// 追加
		enemies_.push_back(newEnemy);
	}

	// カメラの生成
	cameraController_ = new CameraController();
	// カメラの初期化
	cameraController_->Initialize();
	// カメラの追従対象をリセット
	cameraController_->SetTarget(player_);
	// カメラをリセット（瞬間合わせ）
	cameraController_->Reset();
	// カメラの移動範囲を指定
	cameraController_->SetMovableArea_(cameraArea);
}

void GameScene::Update() { 

	switch (phase_) {
	case Phase::kPlay:
		// ゲームプレイフェーズの処理
		// 天球の更新
		skydome_->Update();

		// 自キャラの更新
		player_->Update();

		// 敵キャラの更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

			// デバッグカメラを更新
		debugCamera_->Update();

		if (player_->IsDead()) {
			// 死亡演出フェーズに切り替え
			phase_ = Phase::kDeath;
			// 自キャラの座標を取得
			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			// パーティクルの初期化
			deathParticles_->Initialize(modelDeathParticles_, &viewProjection_, deathParticlesPosition);

		}

#ifdef _DEBUG
		// カメラモード切り替え
		if (input_->TriggerKey(DIK_SPACE)) {
			isDebugCameraActive_ = true;
		}
#endif
		if (isDebugCameraActive_) {
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
			viewProjection_.TransferMatrix();
		} else {
			viewProjection_.matView = cameraController_->GetViewProjection().matView;
			viewProjection_.matProjection = cameraController_->GetViewProjection().matProjection;
			// ビュープロジェクション行列の転送
			viewProjection_.TransferMatrix();
		}

		// カメラを更新
		cameraController_->Update();

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				worldTransformBlock->UpdateMatrix();
			}
		}

		// 全ての当たり判定を行う
		CheckAllCollisions();

		break;
	case Phase::kDeath:
		// デス演出フェーズの処理
		// 天球の更新
		skydome_->Update();

		// 敵キャラの更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		// パーティクルの更新
		deathParticles_->Update();

		// カメラを更新
		cameraController_->Update();

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				worldTransformBlock->UpdateMatrix();
			}
		}

		// ゲームシーンの終了条件
		if (deathParticles_ && deathParticles_->IsFinished()) {
			finished_ = true;
		}

		break;
//	case Phase::kNext:
//		break;

	case Phase::kClear:
		break;
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	switch (phase_) {
	case Phase::kPlay:
		// ゲームプレイフェーズの処理
		// 天球の描画
		skydome_->Draw();

		// 自キャラの描画
		player_->Draw();

		// 敵キャラの描画
		for (Enemy* enemy : enemies_) {
			enemy->Draw();
		}

		// ブロックの描画
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				modelBlock_->Draw(*worldTransformBlock, viewProjection_);
			}
		}

		break;
	case Phase::kDeath:
		// デス演出フェーズの処理
		// 天球の描画
		skydome_->Draw();

		// 敵キャラの描画
		for (Enemy* enemy : enemies_) {
			enemy->Draw();
		}

		// パーティクルの描画
		deathParticles_->Draw();

		// ブロックの描画
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				modelBlock_->Draw(*worldTransformBlock, viewProjection_);
			}
		}
		break;
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::GenerateBlocks() {
	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 列数を設定（縦方向のブロック数）
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		// 1列の要素数を設定（横方向のブロック数）
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

// 全ての当たり判定を行う
void GameScene::CheckAllCollisions() {
	#pragma region 自キャラと敵キャラの当たり判定
	{
		// 判定対象1と2の座標
		AABB aabb1, aabb2;

		// 自キャラの座標
		aabb1 = player_->GetAABB();

		// 自キャラと敵弾全ての当たり判定
		for (Enemy* enemy : enemies_) {
			// 敵弾の座標
			aabb2 = enemy->GetAABB();

			// AABB同士の交差判定
			if (isCollisionAABB(aabb1, aabb2)) {
				// 自キャラの衝突時コールバックを呼び出す
				player_->OnCollision(enemy);
				// 敵弾の衝突時コールバックを呼び出す
				enemy->OnCollision(player_);
			}
		}
	}
	#pragma endregion
}

// フェーズの切り替え
void GameScene::ChangePhase() { 
	switch (phase_) {
	case Phase::kPlay:
		// ゲームプレイフェーズの処理
		if (player_->IsDead()) {
			// 死亡演出フェーズに切り替え
			phase_ = Phase::kDeath;
			// 自キャラの座標を取得
			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			// パーティクルの初期化
			deathParticles_->Initialize(modelDeathParticles_, &viewProjection_, deathParticlesPosition);
		}
		break;
	case Phase::kDeath:
		// デス演出フェーズの処理
		break;
	} 
}