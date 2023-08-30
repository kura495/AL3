#include <cassert>
#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include"PrimitiveDrawer.h"
#include"AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//プレイヤークラス
	modelFighterBody_.reset(Model::CreateFromOBJ("float_Body", true));
	modelFighterHead_.reset(Model::CreateFromOBJ("float_Head", true));
	modelFighterL_arm_.reset(Model::CreateFromOBJ("float_L_arm", true));
	modelFighterR_arm_.reset(Model::CreateFromOBJ("float_R_arm", true));
	modelFighterWeapon_.reset(Model::CreateFromOBJ("weapon", true));
	std::vector<Model*> playerModels = {
	    modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(),modelFighterR_arm_.get(), modelFighterWeapon_.get()};
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels);
	//自キャラを追従するカメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initalize();
	followCamera_->SetTarget(&player_->GetWorldTransform());
	//プレイヤーに追従カメラのビュープロジェクションを登録
	player_->SetViewProjection(&followCamera_->GetViewProjection());
	//敵キャラ
	enemyModel_.reset(Model::CreateFromOBJ("enemy", true));
	std::vector<Model*> enemyModels = { enemyModel_.get()};
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(enemyModels);

	//天球
	skydomeModel.reset(Model::CreateFromOBJ("skydome", true));
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel.get());
	//地面
	groundModel.reset(Model::CreateFromOBJ("ground", true));
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel.get());
	//コリジョン
	collisionManager_ = std::make_unique<CollisionManager>();


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
	//プレイヤー
	player_->Update();
	//敵
	if (enemy_.get()->GetIsAlive()) {
		enemy_->Update();
	}

	//天球
	skydome_->Update();
	//地面
	ground_->Update();
	//プレイヤー追従カメラ
	followCamera_->Update();
	viewProjection_.matView = followCamera_->GetViewProjection().constMap->view;
	viewProjection_.matProjection = followCamera_->GetViewProjection().constMap->projection;
	viewProjection_.TransferMatrix();
	//当たり判定
	CheckAllCollisions();

	//デバッグカメラ
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
	//敵
	if (enemy_.get()->GetIsAlive()) {
		enemy_->Draw(viewProjection_);
	}
	
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

void GameScene::CheckAllCollisions() {
	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 自機のコライダーを登録
	// TODO : コメントアウト消す
	collisionManager_->AddCollider(player_.get());
	// 敵機のコライダーを登録
	collisionManager_->AddCollider(enemy_.get());
	// 自機の弾
	for (PlayerBullet* bullet : playerBullets) {
		collisionManager_->AddCollider(bullet);
	}
	collisionManager_->CheckAllCollisions();
	collisionManager_->ClearCollider();

}
