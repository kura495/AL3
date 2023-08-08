#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Calc/Matrix.h"
#include"Utility/ImGuiSupport.h"
#include"PrimitiveDrawer.h"
#include<imgui.h>
#include<vector>
class RailCamera {
public:
	void Initialize(ViewProjection viewProjection);
	void Update();
	
	void DrawRailLine(); 
	
	const ViewProjection& GetViewProjection() const { return viewProjection_; }
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	//レールカメラの移動曲線
	//スプライン曲線制御点(通過点)
	std::vector<Vector3> controlPoints_;
	
};