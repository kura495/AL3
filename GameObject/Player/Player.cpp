#include "GameObject/Player/Player.h"
#include<cassert>

Player::Player() {}

Player::~Player() { 
	for (PlayerBullet* bullet_ : bullets_) {
		delete bullet_;
	}
	delete sprite2DReticle_;
}

void Player::Initialize(Model* model, uint32_t textureHandle,Vector3 Position) { 
	assert(model);
	textureHandle_ = textureHandle;
	model_ = model;
	worldTransform_.Initialize();
	input_ = Input::GetInstance();
	//プレイヤーをずらす
	worldTransform_.translation_ = Position;
	worldTransform_.UpdateMatrix();
	//当たり判定の属性とマスク設定
	SetcollitionAttribute(kCollitionAttributePlayer);
	SetcollisionMask(~kCollitionAttributePlayer);
	//レティクル
	ReticleModel = Model::Create();
	worldTransform3DReticle_.Initialize();
	uint32_t textureReticle = TextureManager::Load("reticle.png");
	sprite2DReticle_ = Sprite::Create(textureReticle, {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f});
	
	//マウス座標
	hwnd = WinApp::GetInstance()->GetHwnd();
	GetMauseCursur();
}

void Player::Updete(const ViewProjection& viewProjection) { 
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
	//レティクル
	SetReticle();
	reticle3DWorldToreticle2DScreen(viewProjection);
	GetMauseCursur();
	SetMauseReticle(viewProjection);
	// 玉の発射
	Attack();
	//玉の更新
		for (PlayerBullet* bullet_ : bullets_) {
		bullet_->Update();
		}
	
	//ImGui
	#pragma region ImGui 
	ImGui::Begin("Player");
	ImGui::Text("Reticle");
	ImGui::Text("2DReticle:(%f,%f)", mousePosition.x, mousePosition.y);
	ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y,posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y,posFar.z);
	ImGui::Text(
	    "3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
	    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	ImGui::End();
	#pragma endregion
	worldTransform_.UpdateMatrix();
}

void Player::Draw(const ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection,textureHandle_);
	for (PlayerBullet* bullet_ : bullets_) {
		bullet_->Draw(viewProjection);
	}
	ReticleModel->Draw(worldTransform3DReticle_, viewProjection);
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;
	worldTransform_.UpdateMatrix();
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Player::OnCollision() { 
	return; }

void Player::SetParent(const WorldTransform* parent) { 
	worldTransform_.parent_ = parent; 
}

void Player::DrawUI() { 
	sprite2DReticle_->Draw();
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
		Vector3 velocity(0, 0, 0);
		velocity.x = worldTransform3DReticle_.translation_.x - GetWorldPosition().x;
		velocity.y = worldTransform3DReticle_.translation_.y - GetWorldPosition().y;
		velocity.z = worldTransform3DReticle_.translation_.z - GetWorldPosition().z;
		velocity = Normalize(velocity);
		velocity.x *= kBulletSpeed;
		velocity.y *= kBulletSpeed;
		velocity.z *= kBulletSpeed;
		//速度ベクトルを自機の向きに合わせて回転
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);
		bullets_.push_back(newBullet);
	}
	
}
	/// <summary>
	/// レティクルの位置を決める
	/// </summary>
void Player::SetReticle() { 
	const float kDistancePlayerTo3DReticle = 50.0f;
	//自機から3Dレティクルへの距離(Z+向き)
	Vector3 offset = {0, 0, 1.0f};
	//自機のワールド行列の回転を反映
	offset = TransformNormal(offset,worldTransform_.constMap->matWorld);
	//ベクトルの長さを整える
	offset = Normalize(offset);
	offset.x *= kDistancePlayerTo3DReticle;
	offset.y *= kDistancePlayerTo3DReticle;
	offset.z *= kDistancePlayerTo3DReticle;
	//3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_.x = GetWorldPosition().x + offset.x;
	worldTransform3DReticle_.translation_.y = GetWorldPosition().y + offset.y;
	worldTransform3DReticle_.translation_.z = GetWorldPosition().z + offset.z;
	worldTransform3DReticle_.UpdateMatrix();
}
	/// <summary>
	/// 3Dレティクルを2Dレティクルに変換
	/// </summary>
	/// <param name="viewProjection"></param>
void Player::reticle3DWorldToreticle2DScreen(const ViewProjection& viewProjection) {
		// 3Dレティクルのワールド行列からワールド座標を取得
		positionReticle = {
		    worldTransform3DReticle_.matWorld_.m[3][0], worldTransform3DReticle_.matWorld_.m[3][1],
		    worldTransform3DReticle_.matWorld_.m[3][2]};
		// ビューポート行列
		Matrix4x4 matViewport =
		    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
		// ビュー行列とプロジェクション行列、ビューポート行列を合成
		Matrix4x4 matViewProjectionViewport =
		    Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));
		// ワールド→スクリーン座標変換(3D→2D)
		positionReticle = Transformed(positionReticle, matViewProjectionViewport);
		// スプライトのレティクルに座標を設定
		sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
}
	/// <summary>
	/// マウスカーソルを取得
	/// </summary>
void Player::GetMauseCursur() { 
	//マウス座標(スクリーン座標)を取得
	GetCursorPos(&mousePosition);
	//クライアントエリア座標に変換する
	ScreenToClient(hwnd,&mousePosition);
	sprite2DReticle_->SetPosition(Vector2((float)mousePosition.x, (float)mousePosition.y));
}
	/// <summary>
	/// マウスカーソルの位置にレティクルをおく
	/// </summary>
	/// <param name="viewProjection"></param>
void Player::SetMauseReticle(const ViewProjection& viewProjection) {
	// ビューポート行列
	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// ビュー行列とプロジェクション行列、ビューポート行列を合成
	Matrix4x4 matViewProjectionViewport =
	    Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));
	// 合成行列の逆行列を計算
	Matrix4x4 matInverseVPV = Inverse(matViewProjectionViewport);
	// スクリーン座標
	posNear = Vector3((float)mousePosition.x, (float)mousePosition.y, 0);
	posFar = Vector3((float)mousePosition.x, (float)mousePosition.y, 1);
	// スクリーン座標系からワールド座標系
	posNear = Transformed(posNear, matInverseVPV);
	posFar = Transformed(posFar, matInverseVPV);
	// マウスレイの方向
	Vector3 mouseDirection = Subtract(posFar,posNear);
	mouseDirection = Normalize(mouseDirection);
	// カメラから標準オブジェクトの距離
	const float kDistanceTestObject = 50.0f;
	//mouseDirection.z -= kDistanceTestObject;
	worldTransform3DReticle_.translation_.x = posNear.x + mouseDirection.x * kDistanceTestObject;
	worldTransform3DReticle_.translation_.y = posNear.y + mouseDirection.y * kDistanceTestObject;
	worldTransform3DReticle_.translation_.z = posNear.z + mouseDirection.z * kDistanceTestObject;
	worldTransform3DReticle_.UpdateMatrix();
}
