#include "CameraController.h"
#include "Player.h"
#include <algorithm>
#include <MathUtilityForText.h>

Vector3 CameraController::Lerp(Vector3 start, Vector3 end, float t) {
	Vector3 result;
	result.x = (1.0f - t) * start.x + t * end.x;
	result.y = (1.0f - t) * start.y + t * end.y;
	result.z = (1.0f - t) * start.z + t * end.z;
	return result;
}

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
	goalCoord_ = targetWorldTransform.translation_ + targetOffset_ + targetVelocity * kVelocityBias;

	// 座標補間によりゆったり追従
	viewProjection_.translation_ = Lerp(viewProjection_.translation_, goalCoord_, kInterpolationRate);
	
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
	viewProjection_.translation_ = targetWorldTransform.translation_ + targetOffset_;
}