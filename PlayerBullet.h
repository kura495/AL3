#pragma once
#include"Model.h"
#include "WorldTransform.h"
#include"WorldTransformEx.h"
class PlayerBullet {
public:
	PlayerBullet();
	~PlayerBullet();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void Draw(const ViewProjection viewProjection);

private:

	void Move();

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	WorldTransformEx worldTransformEx_;
};

