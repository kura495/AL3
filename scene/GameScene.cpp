#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "ImGuiManager.h"
#include"PrimitiveDrawer.h"
#include"AxisIndicator.h"
#include"Calc/Matrix.h"
#include"AxisIndicator.h"
#include<fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete model_;
	delete player_;
	delete debugCamera_;
	for (Enemy* enemy_ : enemys_) {
		delete enemy_;
	}
	delete modelSkydome_;
	delete skydome_;
	delete railCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	model_ = Model::Create();
	viewProjection_.Initialize();

	textureHandle_ = TextureManager::Load("sample.png");
	
	//自キャラ
	player_ = new Player();
	player_->Initialize(model_, textureHandle_, {0,0,25});
	
	collisionManager_ = new CollisionManager();

	//レールカメラ
	railCamera_ = new RailCamera;
	railCamera_->Initialize(viewProjection_);
	//親子関係
	player_->SetParent(&railCamera_->GetWorldTransform());
	//天球
	modelSkydome_ = Model::CreateFromOBJ("skydome",true);
	skydome_ = new Skydome;
	skydome_->Initialize(modelSkydome_);
	//レティクルのテクスチャ
	TextureManager::Load("reticle.png");
	enemyPopCommands = LoadCSVData("CSV/enemyPop.csv");

	debugCamera_ = new DebugCamera(1280,720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() { 
	// デバッグとImGui 
	
    railCamera_->Update();
	viewProjection_.matView = railCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
	viewProjection_.TransferMatrix();
	UpdateEnemyPopCommands();
	player_->Updete(viewProjection_);
	EnemyUpdate();
	skydome_->Update();
	CheckAllCollisions();
	ImGui::Begin("Control");
	ImGui::Text("DebugCamera : 0\n");
	ImGui::Text("PlayerMove : ARROWKEY\n");
	ImGui::Text("PlayerRotate : A D\n");
	ImGui::Text("PlayerShot : SPACE\n");
	ImGui::End();
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
	EnemyDraw();
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
	player_->DrawUI();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void GameScene::CheckAllCollisions() {
	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemyBullets_;
	// 自機のコライダーを登録
	collisionManager_->AddCollider(player_);
	// 敵機のコライダーを登録
	for (Enemy* enemy_ : enemys_) {
		collisionManager_->AddCollider(enemy_);
	}
	
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

std::stringstream GameScene::LoadCSVData(const std::string& filename) {
	std::ifstream file;
	file.open(filename);
	assert(file.is_open());
	// ファイルの内容を文字列ストリームにコピー
	std::stringstream Commands;
	Commands << file.rdbuf();
	// ファイルを閉じる
	file.close();
	return Commands;
}

#pragma region Enemy
void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) { 
	enemyBullets_.push_back(enemyBullet);
}
void GameScene::EnemyUpdate() {
	for (Enemy* enemy_ : enemys_) {
		enemy_->Update();
	}
	enemys_.remove_if([](Enemy* enemy_) {
		if (enemy_->Isdead()) {
			delete enemy_;
			return true;
		}
		return false;
	});
	for (EnemyBullet* bullet_ : enemyBullets_) {
		bullet_->Update();
	}
	enemyBullets_.remove_if([](EnemyBullet* bullet_) {
		if (bullet_->Isdead()) {
			delete bullet_;
			return true;
		}
		return false;
	});
}
void GameScene::EnemyDraw() {
	for (Enemy* enemy_ : enemys_) {
		enemy_->Draw(viewProjection_);
	}
	for (EnemyBullet* bullet_ : enemyBullets_) {
		bullet_->Draw(viewProjection_);
	}
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
	enemy_->Initialize(model_, position);
	// EnemyにPlayerのアドレスを渡す
	enemy_->SetPlayer(player_);
	enemy_->SetGameScene(this);
	// 敵
	enemys_.push_back(enemy_);
}
#pragma endregion 敵


