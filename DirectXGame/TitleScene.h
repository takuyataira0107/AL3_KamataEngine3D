#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include <Model.h>
#include <Sprite.h>
#pragma once

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene {
public:
	/// <summary>
	/// コンストクラタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// デストクラタ
	/// </summary>
	~TitleScene();

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

	// 終了フラグ
	bool finished_ = false;

	uint32_t textureHandle_ = 0;
	Sprite* sprite_ = nullptr;
};
