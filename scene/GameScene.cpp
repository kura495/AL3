#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "ImGuiManager.h"
#include"PrimitiveDrawer.h"
#include"AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//textureHandle_ = TextureManager::Load("sample.png");
	//プレイヤークラス
	playerModel_.reset(Model::CreateFromOBJ("player", true));
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModel_.get());
	//天球
	skydomeModel.reset(Model::CreateFromOBJ("skydome", true));
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel.get());
	//地面
	groundModel.reset(Model::CreateFromOBJ("ground", true));
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel.get());

	viewProjection_.Initialize();
	viewProjection_.translation_.y = 5.0f;
	viewProjection_.UpdateMatrix();
	//デバッグカメラ
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);
	//軸表示
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() { 
	player_->Updete();
	skydome_->Update();
	ground_->Update();

	#ifdef _DEBUG
	if (input_->PushKey(DIK_LALT)) {
		isDebugCameraActive_ = true;
	} else {
		isDebugCameraActive_ = false;
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

	//プレイヤー
	player_->Draw(viewProjection_);
	//天球
	skydome_->Draw(viewProjection_);
	//地面
	ground_->Draw(viewProjection_);

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
