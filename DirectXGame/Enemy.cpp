#include "Enemy.h"
#include "MathUtilityForText.h"


void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	// 引数で受け取った モデル をメンバ変数に記録
	assert(model);   // NULLポインタチェック
	model_ = model; 

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 初期座標の設定
	worldTransform_.translation_ = position;
	// 敵の向いている方向
	worldTransform_.rotation_.y = 3.0f * std::numbers::pi_v<float> / 2.0f;
	
	// 引数で受け取った ビュープロジェクション をメンバ変数に記録
	viewProjection_ = viewProjection;

	// 速度を設定する
	velocity_ = {-kWalkSpeed, 0, 0};

	// タイマーを初期化
	walkTimer_ = 0.0f;
}

void Enemy::Update() { 
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	// 移動
	worldTransform_.translation_ += velocity_;

	// タイマーを加算
	walkTimer_ += 1.0f / 60.0f;

	// 回転アニメーション
	float param = std::sin((2.0f * std::numbers::pi_v<float>)*walkTimer_ / kWalkMotionTime);
	float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = fLerp(kWalkMotionAngleStart, kWalkMotionAngleEnd, radian);

	// 行列計算
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_);
}