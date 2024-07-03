#include "DeathParticles.h"
#include "MathUtilityForText.h"
#include <assert.h>
#include <algorithm>

void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) { 
	// 引数で受け取った モデル をメンバ変数に記録
	assert(model); // NULLポインタチェック
	model_ = model; 

	// 引数で受け取った ビュープロジェクション をメンバ変数に記録
	viewProjection_ = viewProjection;

	// ワールド変換の初期化
	for (auto& worldTransform : worldTransform_) {
		worldTransform.Initialize();
		// 初期座標の設定
		worldTransform.translation_ = position;
	}

	// カラーの初期化
	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
}

void DeathParticles::Update() { 
	// 終了なら何もしない
	if (isFinished_) {
		return;
	}

	for (uint32_t i = 0; i < kSubdivision; ++i) {
		// 基本となる速度ベクトル
		Vector3 velocity = {kSpeed, 0, 0};
		// 回転角を計算する
		float angle = kAngleUnit * i;
		// Z軸まわり回転行列
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
		// 基本ベクトルを回転させて速度ベクトルを得る
		velocity = Transform(velocity, matrixRotation);
		// 移動処理
		worldTransform_[i].translation_ += velocity;
	}

	// カウンターを1フレーム分の秒数進める
	counter_ += 1.0f / 60.0f;
	// 存続時間の上限に達したら
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		// 終了扱いにする
		isFinished_ = true;
	}

	// カラー
	color_.w = std::clamp(kDuration - counter_, 0.0f, 1.0f);
	// 色変更オブジェクトに色の数値を設定する
	objectColor_.SetColor(color_);
	// 色変更オブジェクトをVRAMに転送
	objectColor_.TransferMatrix();

	for (auto& worldTransform : worldTransform_) {
		// 行列を定数バッファに転送
		worldTransform.TransferMatrix();
		// 行列計算
		worldTransform.UpdateMatrix();
	}
}

void DeathParticles::Draw() {
	// 終了なら何もしない
	if (isFinished_) {
		return;
	}

	for (auto& worldTransform : worldTransform_) {
		// 3Dモデルを描画
		model_->Draw(worldTransform, *viewProjection_, &objectColor_);
	}
}