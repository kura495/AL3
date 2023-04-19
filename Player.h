#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include<assert.h>

/// <summary>
/// 自キャラ
/// <summary>
class Player {
public:
	/// <summary>
	/// 初期化
	/// <summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model*model,uint32_t textureHandlle);

	/// <summary>
	/// 自キャラ
	/// <summary>
	void Update();

	/// <summary>
	/// 自キャラ
	/// <summary>
	void Draw();

private:
	//ワールドトランスフォーム
	WorldTransform* worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};
