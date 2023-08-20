#pragma once
#include "Input.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Calc/Matrix.h"
#include "Calc/Vector.h"
class FollowCamera {
public:
	void Initalize();
	void Update();

	void SetTarget(const WorldTransform* target) { target_ = target; };
	const ViewProjection& GetViewProjection() { return viewProjection_; }

private:
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//追従対象
	const WorldTransform* target_ = nullptr;
	// ゲームパッド
	XINPUT_STATE joyState;

};