#include"Player.h"


void Player::Initialize(const std::vector<Model*>& models) {
	//基底クラスの初期化
	BaseCharacter::Initialize(models);
	
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	SetParent(&worldTransformBody_);
	worldTransformBody_.parent_ = &worldTransform_;

}

void Player::Update() { 
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
	UpdateFloatingGimmick();

	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection) {
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);

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
	

