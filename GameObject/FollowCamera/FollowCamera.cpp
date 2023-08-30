#include "FollowCamera.h"

void FollowCamera::Initalize() { 

	viewProjection_.Initialize();
}

void FollowCamera::Update() {
	if (target_) {
		// 追従対象からカメラまでのオフセット
		// TODO : offsetを{0.0f, 5.0f, -10.0f}
		Vector3 offset = {0.0f, 5.0f, -20.0f};
		// カメラの角度から回転行列を計算
		Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_.rotation_);

		// オフセットをカメラの回転に合わせて回転
		offset = TransformNormal(offset, rotateMatrix);
		// 座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = Add(target_->translation_, offset);
	}
	//スティックでのカメラ回転
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.sThumbRX <= 30000 && joyState.Gamepad.sThumbRX >= -30000) {
			kRadian = 0.02f;
		} else {
			kRadian = 0.04f;
		}

		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kRadian;
	}
	
	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();
}
