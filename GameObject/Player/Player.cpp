#include"Player.h"

void Player::Initialize(const std::vector<Model*>& models) {
	//基底クラスの初期化
	BaseCharacter::Initialize(models);
	// 腕の座標
	worldTransformL_arm_.translation_.y = 1.0f;
	worldTransformR_arm_.translation_.y = 1.0f;
	worldTransformWeapon_.translation_.y = 2.0f;

	// 当たり判定とマスク設定
	SetRadius(Radius);
	SetcollitionAttribute(kCollitionAttributePlayer);
	SetcollisionMask(~kCollitionAttributePlayer & ~kCollitionAttributePlayerBullet);

	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	worldTransformWeapon_.Initialize();
	SetParent(&worldTransformBody_);
	worldTransformBody_.parent_ = &worldTransform_;
	//レティクル
	//ReticleModel.reset(Model::Create());
	ReticleModel.push_back(Model::CreateFromOBJ("Reticle", true));
	ReticleModel.push_back(Model::CreateFromOBJ("Reticle", true));
	worldTransform3DReticle_0.Initialize();
	worldTransform3DReticle_1.Initialize();
}

void Player::Update() { 
	//RBを押したらAttackBehavior
	if(joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		if (attackAnimationFrame == 0) {
			behaviorRequest_ = Behavior::kAttack;
		}
	}
	if (behaviorRequest_) {
		// ふるまいの変更
		behavior_ = behaviorRequest_.value();
		// 各振る舞いごとの初期化を実行
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			break;
		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		}
		// ふるまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
	//ふるまいの更新処理
	switch (behavior_) {
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
	case Behavior::kAttack:
		BehaviorRootUpdate();
		BehaviorAttackUpdate();
		break;
	}
	BaseCharacter::Update();
	Set3DReticle(worldTransform3DReticle_0, 10.0f);
	Set3DReticle(worldTransform3DReticle_1, 20.0f);

	// デスフラグが立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	
	// 弾の更新
	for (PlayerBullet* bullet_ : bullets_) {
		bullet_->Update();
	}

	ImGui();
	
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformWeapon_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection) {
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);

	for (PlayerBullet* bullet_ : bullets_) {
		bullet_->Draw(viewProjection);
	}

	ReticleModel[0]->Draw(worldTransform3DReticle_0, viewProjection);
	ReticleModel[1]->Draw(worldTransform3DReticle_1, viewProjection);
}

void Player::OnCollision() { 
	return;
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;
	worldTransform_.UpdateMatrix();
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Player::ImGui() {
	ImGui::Begin("Player");
	ImGui::SliderFloat3("Head", &worldTransformHead_.translation_.x, 0.0f, 10.0f);
	ImGui::SliderFloat3("ArmL", &worldTransformL_arm_.translation_.x, 0.0f, 10.0f);
	ImGui::SliderFloat3("ArmR", &worldTransformR_arm_.translation_.x, 0.0f, 10.0f);
	ImGui::SliderInt("cycle", &floatcycle_, 1, 120);
	ImGui::SliderFloat("Amplitude", &floatingAmplitude_, 0.0f, 10.0f);
	ImGui::InputInt("attackAnimationFrame", &attackAnimationFrame, 0,1);
	ImGui::End();
}

void Player::Shot() {
		// 玉の速度
		const float kBulletSpeed = 1.0f;

		Vector3 worldPos = GetWorldPosition();
		Vector3 velocity = move;
		velocity = Normalize(velocity);
		velocity.x *= kBulletSpeed;
		velocity.y *= kBulletSpeed;
		velocity.z *= kBulletSpeed;
		// 速度ベクトルを自機の向きに合わせて回転
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(GetWorldPosition(), velocity);
	    bullets_.push_back(newBullet);

}

void Player::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransformHead_.parent_ = parent;
	worldTransformL_arm_.parent_ = parent;
	worldTransformR_arm_.parent_ = parent;
	worldTransformWeapon_.parent_ = parent;
}

void Player::InitializeFloatingGimmick() {
	floatingParameter_ = 0.0f; 
}

void Player::UpdateFloatingGimmick() {
	
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

void Player::BehaviorRootUpdate() {
	// ゲームパッドの状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.sThumbLX != 0 || joyState.Gamepad.sThumbLY != 0) {
			// 速さ
			const float speed = 0.3f;
			// 移動量
			move = {
			    (float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
			    (float)joyState.Gamepad.sThumbLY / SHRT_MAX};
			// 正規化をして斜めの移動量を正しくする
			move.x = Normalize(move).x * speed;
			move.y = Normalize(move).y * speed;
			move.z = Normalize(move).z * speed;
			// カメラの正面方向に移動するようにする
			// 回転行列を作る
			Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);
			// 移動ベクトルをカメラの角度だけ回転
			move = TransformNormal(move, rotateMatrix);
			// 移動
			worldTransform_.translation_ = Add(worldTransform_.translation_, move);
			// プレイヤーの向きを移動方向に合わせる
			// playerのY軸周り角度(θy)
			worldTransform_.rotation_.y = std::atan2(move.x, move.z);
		}
	}
	UpdateFloatingGimmick();
}

void Player::BehaviorAttackInitialize() {
	attackAnimationFrame = 0;
	Shot();
}

void Player::BehaviorAttackUpdate() {
	attackAnimationFrame++;
	if (attackAnimationFrame > kBulletinterval) {
		behaviorRequest_ = Behavior::kRoot;
		attackAnimationFrame = 0;
	}
}

	/// <summary>
/// レティクルの位置を決める
/// </summary>
void Player::Set3DReticle(WorldTransform& worldTransform3DReticle_,const float ReticleDistanse) {
	const float kDistancePlayerTo3DReticle = ReticleDistanse;
	// 自機から3Dレティクルへの距離(Z+向き)
	Vector3 offset = {0.0f, 0.0f, 1.0f};
	// 自機のワールド行列の回転を反映
	offset = TransformNormal(offset, worldTransform_.constMap->matWorld);
	// ベクトルの長さを整える
	offset = Normalize(offset);
	offset.x *= kDistancePlayerTo3DReticle;
	offset.y *= kDistancePlayerTo3DReticle;
	offset.z *= kDistancePlayerTo3DReticle;
	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_.x = GetWorldPosition().x + offset.x;
	worldTransform3DReticle_.translation_.y = GetWorldPosition().y + offset.y + 1.0f;
	worldTransform3DReticle_.translation_.z = GetWorldPosition().z + offset.z;
	worldTransform3DReticle_.UpdateMatrix();
}

