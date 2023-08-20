#pragma once
#include <vector>
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
class BaseCharacter {
protected:
	//モデルデータ配列
	std::vector<Model*> models_;
	//ワールド変換データ
	WorldTransform worldTransform_;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models"></param>
	virtual void Initialize(const std::vector<Model*>&models);
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="viewProjection"></param>
	virtual void Draw(const ViewProjection& viewProjection);
	/// <summary>
	/// ワールド変換データを取得
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:

};
