#include "GameObject/Player/Player.h"
#include<cassert>

Player::Player() {}

Player::~Player() { 
	for (PlayerBullet* bullet_ : bullets_) {
		delete bullet_;
	}
}

void Player::Initialize(Model* model, uint32_t textureHandle) { 
	assert(model);
	textureHandle_ = textureHandle;
	model_ = model;
	worldTransform_.Initialize();
	input_ = Input::GetInstance();
}

void Player::Updete() { 
	//デスフラグが立った玉を削除
	bullets_.remove_if([](PlayerBullet* bullet) { 
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	//キャラの移動
	Move();
	//キャラの回転
	Rotate();
	//玉の発射
	Attack();
	//玉の更新
		for (PlayerBullet* bullet_ : bullets_) {
		bullet_->Update();
		}
	

	//ImGuiの準備
	float point[Vector3D] = {worldTransform_.translation_.x, worldTransform_.translation_.y,worldTransform_.translation_.z};
	ImGui::Begin("Player");
	ImGui::SliderFloat3("Point", point, -30.0f, 30.0f);
	ImGui::End();
	worldTransform_.translation_.x = point[x];
	worldTransform_.translation_.y = point[y];
	worldTransform_.translation_.z = point[z];

	worldTransform_.UpdateMatrix();
}

void Player::Draw(const ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_,textureHandle_);
	for (PlayerBullet* bullet_ : bullets_) {
		bullet_->Draw(viewProjection_);
	}
	
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;
	worldTransform_.UpdateMatrix();
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

//private関数

void Player::Move() {
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	//キャラクターの移動の速さ
	const float CharacterSpeed = 0.2f;
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= CharacterSpeed;
	} else if(input_->PushKey(DIK_RIGHT)) {
		move.x += CharacterSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += CharacterSpeed;
	} else if(input_->PushKey(DIK_DOWN)) {
		move.y -= CharacterSpeed;
	}
	//移動制限
	const float kMoveLimitX=30;
	const float kMoveLimitY=15;
	worldTransform_.translation_.x = max(worldTransform_.translation_.x,-kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x,+kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y,-kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y,+kMoveLimitY);
	//加算
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
}

void Player::Rotate() {
	// 回転
	const float RotSpeed = 0.02f;
	// Y軸
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= RotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += RotSpeed;
	}
}
	
void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		//玉の速度
		const float kBulletSpeed = 1.0f;
		//1フレームにつきZ方向に1.0f進む
		Vector3 velocity(0, 0, kBulletSpeed);
		//速度ベクトルを自機の向きに合わせて回転
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);
		bullets_.push_back(newBullet);
	}
	
}
