#include"Player.h"


void Player::Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm) {
	assert(modelBody);
	assert(modelHead);
	assert(modelL_arm);
	assert(modelR_arm);
	modelBody_ = modelBody;
	modelHead_ = modelHead;
	modelL_arm_ = modelL_arm;
	modelR_arm_ = modelR_arm;
	worldTransformBase_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	SetParent(&worldTransformBody_);
	worldTransformBody_.parent_ = &worldTransformBase_;

}

void Player::Updete() { 
	worldTransformBase_.TransferMatrix();

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
		worldTransformBase_.translation_ = Add(worldTransformBase_.translation_, move);
		//プレイヤーの向きを移動方向に合わせる
		//playerのY軸周り角度(θy)
		worldTransformBase_.rotation_.y = std::atan2(move.x, move.z);
		
	}
	UpdateFloatingGimmick();

	worldTransformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection) {
	modelBody_->Draw(worldTransformBody_, viewProjection);
	modelHead_->Draw(worldTransformHead_, viewProjection);
	modelL_arm_->Draw(worldTransformL_arm_, viewProjection);
	modelR_arm_->Draw(worldTransformR_arm_, viewProjection);

}

void Player::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransformHead_.parent_ = parent;
	worldTransformL_arm_.parent_ = parent;
	worldTransformR_arm_.parent_ = parent;
}

void Player::InitializeFloatingGimmick() {
	floatingParameter_ = 0.0f; 
}

void Player::UpdateFloatingGimmick() {
	ImGui::Begin("Player");
	ImGui::SliderFloat3("Head", &worldTransformHead_.translation_.x, 0.0f,10.0f);
	ImGui::SliderFloat3("ArmL", &worldTransformL_arm_.translation_.x, 0.0f, 10.0f);
	ImGui::SliderFloat3("ArmR", &worldTransformR_arm_.translation_.x, 0.0f, 10.0f);
	ImGui::SliderInt("cycle", &floatcycle_,1, 120);
	ImGui::SliderFloat("Amplitude", &floatingAmplitude_, 0.0f, 10.0f);
	ImGui::End();
	// 浮遊移動のサイクル<frame>
	const uint16_t T = (uint16_t)floatcycle_;
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * (float)std::numbers::pi / T;
	// パラメータを1ステップ分加算
	floatingParameter_ += step;
	// 2πを超えたら0に戻す
	floatingParameter_ = (float)std::fmod(floatingParameter_, 2.0f * std::numbers::pi);
	// 浮遊の振幅<m>
	const float floatingAmplitude = floatingAmplitude_;
	// 浮遊を座標に反映
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * floatingAmplitude;

}
	

