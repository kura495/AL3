#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include <memory>
#include "GameObject/Player/Player.h"
#include "GameObject/Skydome/Skydome.h"
#include "GameObject/Ground/Ground.h"

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
	std::unique_ptr<Model> playerModel_ = nullptr;
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
};
