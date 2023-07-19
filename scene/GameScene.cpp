#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "ImGuiManager.h"
#include"PrimitiveDrawer.h"
#include"AxisIndicator.h"
#include"Calc/Matrix.h"
#include"AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete model_;
	delete player_;
	delete debugCamera_;
	delete enemy_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("sample.png");
	model_ = Model::Create();
	viewProjection_.Initialize();
	//自キャラ
	player_ = new Player();
	player_->Initialize(model_,textureHandle_);
	//敵
	enemy_ = new Enemy();
	enemy_->Initialize(model_);
	//EnemyにPlayerのアドレスを渡す
	enemy_->SetPlayer(player_);

	debugCamera_ = new DebugCamera(1280,720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() { 
	player_->Updete();
	enemy_->Update();
	CheckAllCollisions();

	//デバッグとImGui
	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = true;
	}
	#endif
	ImGui::Begin("Control");
	ImGui::Text("DebugCamera : 0\n");
	ImGui::Text("PlayerMove : ARROWKEY\n");
	ImGui::Text("PlayerRotate : A D\n");
	ImGui::Text("PlayerShot : SPACE\n");
	ImGui::End();
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView=debugCamera_->GetViewProjection().constMap->view;
		viewProjection_.matProjection=debugCamera_->GetViewProjection().constMap->projection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}
	
	
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>


	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	

	// 3Dモデルの描画
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);
	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}
void GameScene::CheckAllCollisions() {
	// 判定対象AとBの座標
	Vector3 posA, posB;
	// 自弾リストの取得
	const std::list<PlayerBullet*>& PlayerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& EnemyBullets = enemy_->GetBullets();

#pragma region
	posA = player_->GetWorldPosition();
	for (EnemyBullet* bullet : EnemyBullets) {
		posB = bullet->GetWorldPosition();
		float Length = sqrt(
		    (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
		    (posB.z - posA.z) * (posB.z - posA.z));

		float Radius = 2.0f;
		if (Length <= Radius) {
			// 自キャラの衝突時コールバック
			player_->OnCollision();
			// 敵の弾の衝突時コールバック
			bullet->OnCollision();
		}
	}
#pragma endregion 自キャラと敵弾の当たり判定

#pragma region
	posA = enemy_->GetWorldPosition();
	for (PlayerBullet* bullet : PlayerBullets) {
		posB = bullet->GetWorldPosition();
		float Length = sqrt(
		    (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
		    (posB.z - posA.z) * (posB.z - posA.z));

		float Radius = 2.0f;
		if (Length <= Radius) {
			// 自キャラの衝突時コールバック
			player_->OnCollision();
			// 敵の弾の衝突時コールバック
			bullet->OnCollision();
		}
	}
#pragma endregion 自弾と敵キャラの当たり判定

#pragma region
	
	for (PlayerBullet* Playerbullet : PlayerBullets) {
		posA = Playerbullet->GetWorldPosition();
		for (EnemyBullet* Enemybullet : EnemyBullets) {
			posB = Enemybullet->GetWorldPosition();
			float Length = sqrt(
		    (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
		    (posB.z - posA.z) * (posB.z - posA.z));

			float Radius = 2.0f;
			if (Length <= Radius) {
				// 自キャラの弾の衝突時コールバック
				Playerbullet->OnCollision();
				// 敵の弾の衝突時コールバック
				Enemybullet->OnCollision();
			}

		}
		
	}
#pragma endregion 自弾と敵弾の当たり判定
}
