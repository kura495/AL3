#include"Player.h"
#include<cassert>

void Player::Initialize(Model* model) { 
	assert(model);
	model_ = model;
	worldTransform_.Initialize();

}

void Player::Updete() { 
	worldTransform_.TransferMatrix();

	//ゲームパッドの状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		//速さ
		const float speed = 0.3f;
		//移動量
		Vector3 move{
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX};
		//正規化をして斜めの移動量を正しくする
		move.x =Normalize(move).x * speed;
		move.y =Normalize(move).y * speed;
		move.z =Normalize(move).z * speed;
		//カメラの正面方向に移動するようにする
		//回転行列を作る
		Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);
		//移動ベクトルをカメラの角度だけ回転
		move = TransformNormal(move, rotateMatrix);
		//移動
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
		//プレイヤーの向きを移動方向に合わせる
		//playerのY軸周り角度(θy)
		worldTransform_.rotation_.y = std::atan2(move.x, move.z);
		
	}
	
	worldTransform_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);

}
	

