#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Calc/Matrix.h"
#include <imgui.h>
#include"Utility/ImGuiSupport.h"
class RailCamera {
public:
	void Initialize(ViewProjection viewProjection);
	void Update();
	const ViewProjection& GetViewProjection() const { return viewProjection_; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
};