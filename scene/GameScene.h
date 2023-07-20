#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"GameObject/Player/Player.h"
#include"GameObject/Enemy/Enemy.h"
#include"DebugCamera.h"
#include"Utility/Collider.h"
#include"Utility/CollisionManager.h"
#include"GameObject/Skydome/Skydome.h"
#include"GameObject/RailCamera/RailCamera.h"
    /// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// エネミーの弾登録
	/// </summary>
	/// <param name="enemyBullet"></param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//3Dモデル
	Model* model_ = nullptr;
	//3Dモデル
	Model* modelSkydome_ = nullptr;
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//自キャラ
	Player* player_ = nullptr;
	
	//天球
	Skydome* skydome_=nullptr;
	//レールカメラ
	RailCamera* railCamera_=nullptr;
	//デバッグカメラ有効
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;
	//CorisionManager
	CollisionManager* collisionManager_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	void CheckAllCollisions();
	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();
	void UpdateEnemyPopCommands();
	#pragma region Enemy
	//敵
	std::list<Enemy*> enemys_;
	void EnemyUpdate();
	void EnemyDraw();
	void EnemySpawn(const Vector3& position);
	//敵の弾リスト
	std::list<EnemyBullet*> enemyBullets_;
	//敵発生コマンド
	std::stringstream enemyPopCommands;
	//敵の待機フラグ
	bool enemyWAITflag = false;
	int enemyWAITtime = 0;
#pragma endregion 敵
};
