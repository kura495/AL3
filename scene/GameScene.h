#pragma once
//systems
#include <memory>
//Base
#include "DirectXCommon.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "Utility/LoadCSV.h"
//Components
#include "Audio.h"
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "Utility/CollisionManager.h"
//Object
#include "GameObject/Player/Player.h"
#include "GameObject/Enemy/Enemy.h"
#include "GameObject/Skydome/Skydome.h"
#include "GameObject/Ground/Ground.h"
#include "GameObject/FollowCamera/FollowCamera.h"

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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//3Dモデル
	
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//自キャラ
	std::unique_ptr<Player> player_ = nullptr;
	std::unique_ptr<Model> modelFighterBody_ = nullptr;
	std::unique_ptr<Model> modelFighterHead_ = nullptr;
	std::unique_ptr<Model> modelFighterL_arm_ = nullptr;
	std::unique_ptr<Model> modelFighterR_arm_ = nullptr;
	std::unique_ptr<Model> modelFighterWeapon_ = nullptr;
	//自キャラを追従するカメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;
	//敵キャラ
	std::list<Enemy*> enemys_;
	Enemy* enemy_ = nullptr;
	std::unique_ptr<Model> enemyModel_ = nullptr;
	std::vector<Model*> enemyModels;
	// 敵発生コマンド
	std::stringstream enemyPopCommands;
	// 敵の待機フラグ
	bool enemyWAITflag = false;
	int enemyWAITtime = 0;
	void UpdateEnemyPopCommands();
	void EnemySpawn(const Vector3& position);

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	//天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
	std::unique_ptr<Model> skydomeModel = nullptr;
	//地面
	std::unique_ptr<Ground> ground_	= nullptr;
	std::unique_ptr<Model> groundModel = nullptr;
	//デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	int isDebugCameraActive_ = false;
	//コリジョン管理
	void CheckAllCollisions();
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;
};
