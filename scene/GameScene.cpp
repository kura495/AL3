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
	enemyPopCommands = LoadCSVData("CSV/enemyPop.csv");
	enemyModel_.reset(Model::CreateFromOBJ("enemy", true)); 
	enemyModels = { enemyModel_.get()};

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

	//クリア
	titleHundle = TextureManager::Load("Title.png");
	Title.reset(Sprite::Create(titleHundle, {0, 0}));
	//タイトル
	ClaerHundle = TextureManager::Load("clear.png");
	Clear.reset(Sprite::Create(ClaerHundle, {320, 160}));
	
	//デバッグカメラ
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);
	//軸表示
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	Input::GetInstance()->GetJoystickState(0, joyStateG);
	if (CurrentSceneNumber == TITLE) {
		TitleUpdate();
	}
	if (CurrentSceneNumber == PLAY) {
		PlayUpdate();
	}
	if (CurrentSceneNumber == CLEAR) {
		
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
	
	if (CurrentSceneNumber == PLAY || CurrentSceneNumber == CLEAR) {
		// プレイヤー
		player_->Draw(viewProjection_);
		// 敵
		for (Enemy* enemy : enemys_) {
			enemy->Draw(viewProjection_);
		}
	}
	// 天球
	skydome_->Draw(viewProjection_);
	// 地面
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
	if (CurrentSceneNumber == TITLE) {
		Title.get()->Draw();
	}
	if (CurrentSceneNumber == CLEAR) {
		Clear.get()->Draw();
	}
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (enemyWAITflag) {
		enemyWAITtime--;
		if (enemyWAITtime <= 0) {
			enemyWAITflag = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;
	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);
		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');
		// "//"はコメント行
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}
		if (word.find("POP") == 0) {
			// X座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			// Y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			// Z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());
			// 敵をスポーン
			EnemySpawn(Vector3(x, y, z));
			EnemyCount += 1;
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');
			// 待ち時間
			int32_t waitTime = atoi(word.c_str());
			enemyWAITflag = true;
			enemyWAITtime = waitTime;
			break;
		}
	}
}

void GameScene::EnemySpawn(const Vector3& position) {
	Enemy* enemy_ = new Enemy();
	enemy_->Initialize(enemyModels);
	enemy_->SetPosition(position);
	// 敵
	enemys_.push_back(enemy_);
}

void GameScene::CheckAllCollisions() {
	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 自機のコライダーを登録
	collisionManager_->AddCollider(player_.get());
	// 敵機のコライダーを登録
	for (Enemy* enemy : enemys_) {
		collisionManager_->AddCollider(enemy);
	}
	
	// 自機の弾
	for (PlayerBullet* bullet : playerBullets) {
		collisionManager_->AddCollider(bullet);
	}
	collisionManager_->CheckAllCollisions();
	collisionManager_->ClearCollider();
}

void GameScene::TitleUpdate() { 
	if (joyStateG.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
		CurrentSceneNumber = PLAY;
	}
}

void GameScene::PlayUpdate() {
	Time += 1 / 60;
	// プレイヤー
	player_->Update();
	// 敵
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}

	// デスフラグが立った敵を削除
	enemys_.remove_if([&](Enemy* enemy) {
		if (!enemy->GetIsAlive()) {
			EnemyLeft -= 1;
			EnemyCount -= 1;
			delete enemy;

			return true;
		}
		return false;
	});
	if (EnemyCount < 10) {
		UpdateEnemyPopCommands();
	}

	if (EnemyCount == 0) {
		CurrentSceneNumber = CLEAR;
	}

	// 天球
	skydome_->Update();
	// 地面
	ground_->Update();
	// プレイヤー追従カメラ
	followCamera_->Update();
	viewProjection_.matView = followCamera_->GetViewProjection().constMap->view;
	viewProjection_.matProjection = followCamera_->GetViewProjection().constMap->projection;
	viewProjection_.TransferMatrix();
	// 当たり判定
	CheckAllCollisions();

// デバッグカメラ
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
