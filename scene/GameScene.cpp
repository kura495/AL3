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
	delete modelSkydome_;
	delete skydome_;
	delete railCamera_;
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
	//天球
	modelSkydome_ = Model::CreateFromOBJ("skydome",true);
	skydome_ = new Skydome;
	skydome_->Initialize(modelSkydome_);

	collisionManager_ = new CollisionManager();


	debugCamera_ = new DebugCamera(1280,720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//レールカメラ
	railCamera_ = new RailCamera;
	railCamera_->Initialize(viewProjection_);
	
}

void GameScene::Update() { 
	// デバッグとImGui
	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = true;
	}
if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().constMap->view;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().constMap->projection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}
	#endif

	player_->Updete();
	enemy_->Update();
	skydome_->Update();
	railCamera_->Update();
	viewProjection_.matView = railCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
	viewProjection_.TransferMatrix();
	CheckAllCollisions();
	ImGui::Begin("Control");
	ImGui::Text("DebugCamera : 0\n");
	ImGui::Text("PlayerMove : ARROWKEY\n");
	ImGui::Text("PlayerRotate : A D\n");
	ImGui::Text("PlayerShot : SPACE\n");
	ImGui::End();
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
	skydome_->Draw(viewProjection_);
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
	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();
	// 自機のコライダーを登録
	collisionManager_->AddCollider(player_);
	// 敵機のコライダーを登録
	collisionManager_->AddCollider(enemy_);
	// 自機の弾
	for (PlayerBullet* bullet : playerBullets) {
		collisionManager_->AddCollider(bullet);
	}
	// 敵機の弾
	for (EnemyBullet* bullet : enemyBullets) {
		collisionManager_->AddCollider(bullet);
	}
	collisionManager_->CheckAllCollisions();
	collisionManager_->ClearCollider();
}



