#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include <Model.h>
#include <Sprite.h>
#include "Skydome.h"


#pragma once
class FailureScene {
public:
	/// <summary>
	/// コンストクラタ
	/// </summary>
	FailureScene();

	/// <summary>
	/// デストクラタ
	/// </summary>
	~FailureScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


	// デスフラグの getter
	bool IsFinished() const { return finished_; }

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 終了フラグ
	bool finished_ = false;

	// 天球
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;

	uint32_t textureHandle_ = 0;
	Sprite* sprite_ = nullptr;
};
