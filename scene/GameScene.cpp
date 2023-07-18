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
	CheckAllCollisions();
	
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
	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();
	//コライダー
	std::list<Collider*> colliders_;
	//自機のコライダーを登録
	colliders_.push_back(player_);
	//敵機のコライダーを登録
	colliders_.push_back(enemy_);
	//自機の弾
	for (PlayerBullet* bullet : playerBullets) {
		colliders_.push_back(bullet);
	}
	//敵機の弾
	for (EnemyBullet* bullet : enemyBullets) {
		colliders_.push_back(bullet);
	}
	//リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end();++itrA) {
		//イテレータAからコライダーを取得
		Collider* colliderA=*itrA;
		//イテレータBはイテレータAの次の要素から回す(重複を避ける)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;
			CheckCollisionPair(colliderA, colliderB);
		}
	}

}

void GameScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	// 判定対象AとBの座標
	Vector3 posA, posB;
	posA = colliderA->GetWorldPosition();
	posB = colliderB->GetWorldPosition();
	float Length = sqrt(
	    (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
	    (posB.z - posA.z) * (posB.z - posA.z));
	//コライダーのフィルターの値でビット演算
	if ((colliderA->GetcollitionAttribute() & colliderB->GetcollisionMask())==0 ||
	    (colliderB->GetcollitionAttribute() & colliderA->GetcollisionMask())==0) {
		return;
	}
	if (Length<=colliderA->GetRadius()+colliderB->GetRadius()){
		//コライダーAの衝突時コールバック
		colliderA->OnCollision();
		//コライダーBの衝突時コールバック
		colliderB->OnCollision();
	}
}
