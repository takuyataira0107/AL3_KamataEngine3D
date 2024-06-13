#include "CameraController.h"
#include "Player.h"
#include <algorithm>

void CameraController::Initialize() {
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void CameraController::Update() {
	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	const Vector3& targetVelocity = target_->GetVelocity();
	const Vector3& targetPosition = target_->GetPlayerPosition();

	// 追従対象とオフセットと追従対象からカメラの目標座標を計算
	goalCoord_.x = targetWorldTransform.translation_.x + targetOffset_.x + targetVelocity.x * kVelocityBias;
	goalCoord_.y = targetWorldTransform.translation_.y + targetOffset_.y + targetVelocity.y * kVelocityBias;
	goalCoord_.z = targetWorldTransform.translation_.z + targetOffset_.z + targetVelocity.z * kVelocityBias;

	// 座標補間によりゆったり追従
	viewProjection_.translation_.x = Lerp(viewProjection_.translation_.x, goalCoord_.x, kInterpolationRate);
	viewProjection_.translation_.y = Lerp(viewProjection_.translation_.y, goalCoord_.y, kInterpolationRate);
	viewProjection_.translation_.z = Lerp(viewProjection_.translation_.z, goalCoord_.z, kInterpolationRate);
	
	// 追従対象が画面外に出ないように補正
	viewProjection_.translation_.x = std::clamp(viewProjection_.translation_.x, targetPosition.x + margin.left, targetPosition.x + margin.right);
	viewProjection_.translation_.y = std::clamp(viewProjection_.translation_.y, targetPosition.y + margin.top, targetPosition.y + margin.bottom);

	// 移動範囲制限
	viewProjection_.translation_.x = std::clamp(viewProjection_.translation_.x, movableArea_.left, movableArea_.right);
	viewProjection_.translation_.y = std::clamp(viewProjection_.translation_.y, movableArea_.top, movableArea_.bottom);
	// 行列を更新する
	viewProjection_.UpdateMatrix();
}

void CameraController::Reset() {
	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追従対象とオフセットからカメラの座標を計算
	viewProjection_.translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	viewProjection_.translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	viewProjection_.translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;
}