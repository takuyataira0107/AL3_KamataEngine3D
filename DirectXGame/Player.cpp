#define NOMINMAX
#include "Player.h"
#include <cassert>
#include <numbers>
#include <input.h>
#include <algorithm>
#include "MathUtilityForText.h"
#include "MapChipField.h"
#include "DebugText.h"

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) { 
	// 引数で受け取った モデル をメンバ変数に記録
	assert(model);   // NULLポインタチェック
	model_ = model;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 初期座標の設定
	worldTransform_.translation_ = position;
	// プレイヤーの向いている方向
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	// 引数で受け取った ビュープロジェクション をメンバ変数に記録
	viewProjection_ = viewProjection;
}

void Player::InputMove() {
	// 接地状態
	if (onGround_) {
		// 左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				//	左移動中の右入力
				if (velocity_.x < 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}

				acceleration.x += kAcceleration;

				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}

			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				// 左移動中の左入力
				if (velocity_.x > 0.0f) {
					//	速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}

				acceleration.x -= kAcceleration;

				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			}

			// 加速/減速
			velocity_ += acceleration;
			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			// 非入力時は移動減衰をかける
			velocity_.x *= (1.0f - kAttenuation);
		}

		if (Input::GetInstance()->PushKey(DIK_UP)) {
			// ジャンプ初速
			velocity_.y += kJumpAcceleration;
		}
	} else {
		// 落下速度
		velocity_.y += -kGravityAcceleration;
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) { 
	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},   // kRightBottom
        {-kWidth / 2.0f, -kHeight / 2.0f, 0},   // kLeftBottom
        {+kWidth / 2.0f, +kHeight / 2.0f, 0},   // kRightTop
        {-kWidth / 2.0f, +kHeight / 2.0f, 0}    // kLeftTop
    };
	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void Player::MapCollisionTop(CollisionMapInfo& info) {
	// 上昇あり？
	if (info.move.y <= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 真上の当たり判定を行う
	bool hit = false;
	MapChipField::IndexSet indexSet;
	// 左上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f, 0));
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
		// 天井に当たったことを記録する
		info.ceilingCollision = true;
	}
}

void Player::MapCollisionBottom(CollisionMapInfo& info) {
	// 下降あり？
	if (info.move.y >= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 真下の当たり判定を行う
	bool hit = false;
	MapChipField::IndexSet indexSet;
	// 左下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット
	if (hit) {
		//めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, -kHeight / 2.0f, 0));
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		info.move.y = std::min(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2.0f + kBlank));
		// 地面に当たったことを記録する
		info.landing = true;
	}
}

void Player::MapCollisionRight(CollisionMapInfo& info) {
	if (info.move.x <= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 右の当たり判定を行う
	bool hit = false;
	MapChipField::IndexSet indexSet;
	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop] + Vector3(+kCollisionExtension, 0, 0));
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(+kCollisionExtension, 0, 0));
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(+kWidth / 2.0f, 0, 0));
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = std::max(0.0f, rect.left - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank));
		// 壁に当たったことを判定結果に記録する
		info.hitWall = true;
	}
}

void Player::MapCollisionLeft(CollisionMapInfo& info) {
	if (info.move.x >= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 左の当たり判定を行う
	bool hit = false;
	MapChipField::IndexSet indexSet;
	// 左上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop] + Vector3(-kCollisionExtension, 0, 0));
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 左下点の判定;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(-kCollisionExtension, 0, 0));
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(-kWidth / 2.0f, 0, 0));
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = std::min(0.0f, rect.right - worldTransform_.translation_.x + (kWidth / 2.0f + kBlank));
		// 壁に当たったことを判定結果に記録する
		info.hitWall = true;
	}
}

void Player::MapCollision(CollisionMapInfo& info) {
	MapCollisionTop(info);
	MapCollisionBottom(info);
	MapCollisionRight(info);
	MapCollisionLeft(info);
}

void Player::MoveReflection(const CollisionMapInfo& info) {
	// 移動
	worldTransform_.translation_ += info.move;
}

void Player::HitCeilingCase(const CollisionMapInfo& info) {
	// 天井に当たった？
	if (info.ceilingCollision) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
	}
}

void Player::HitWallCase(const CollisionMapInfo& info) {
	// 壁接触による原則
	if (info.hitWall) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}

void Player::SwitchingOnGround(const CollisionMapInfo& info) {
	// 自キャラ
	if (onGround_) {
		// 接地状態の処理
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		} else {
			// 移動後の4つの角の座標
			std::array<Vector3, kNumCorner> positionsNew;
			for (uint32_t i = 0; i < positionsNew.size(); ++i) {
				positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
			}

			// 落下判定
			MapChipType mapChipType;
			// 真下の当たり判定を行う
			bool hit = false;
			MapChipField::IndexSet indexSet;
			// 左下点の判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0, -kCollisionExtension, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}
			// 右下点の判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0, -kCollisionExtension, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			// 落下開始
			if (!hit) {
				// 空中状態に切り替える
				onGround_ = false;
			}
		}
	} else {
		// 空中状態の処理
		// 着地フラグ
		if (info.landing) {
			// 着地状態に切り替える（落下を止める）
			onGround_ = true;
			// 着地時にX速度を減衰
			velocity_.x *= (1.0f - kAttenuationLanding);
			// Y速度をゼロにする
			velocity_.y = 0.0f;
		}
	}
}


void Player::TurningControl() {
	// 旋回制御
	if (turnTimer_ > 0.0f) {
		if (turnTimer_ < 1.0f) {
			turnTimer_ += 1.0f / 60.0f;
		}
		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = Player::fLerp(turnFirstRotationY_, destinationRotationY, easeInOutCubic(turnTimer_));
	}
}

void Player::Update() {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	//移動入力
	InputMove();

	// 移動量を加味して衝突判定をする
	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.move = velocity_;
	// マップ衝突チェック
	MapCollision(collisionMapInfo);

	// 判定結果を反映して移動させる
	MoveReflection(collisionMapInfo);

	// 天井に接触している場合の処理
	HitCeilingCase(collisionMapInfo);

	// 壁に接触している場合の処理
	HitWallCase(collisionMapInfo);

	// 接地状態の切り替え
	SwitchingOnGround(collisionMapInfo);

	// 旋回制御
	TurningControl();

	// 行列計算
	worldTransform_.UpdateMatrix();
}

void Player::Draw() {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_);
}