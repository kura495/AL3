#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Calc/Matrix.h"
#include <imgui.h>
#include"Utility/ImGuiSupport.h"
class RailCamera {
public:
	void Initialize(const WorldTransform& worldTransform);
	void Update();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
};