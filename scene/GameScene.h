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
#include "Utility/CollisionManager.h"
#include<list>
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
	Model* model_ = nullptr;
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//自キャラ
	Player* player_ = nullptr;
	//敵
	Enemy* enemy_ = nullptr;
	//デバッグカメラ有効
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;
	//CorisionManager
	CollisionManager* collisionManager_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	void CheckAllCollisions();
};
