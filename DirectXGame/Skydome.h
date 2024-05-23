#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"


/// <summary>
/// 天球
/// </summary>
class Skydome {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュー変換データ
	ViewProjection* viewProjection_ = nullptr;
	// モデル
	Model* model_ = nullptr;

};
