#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Calc/Matrix.h"
#include"Calc/Vector.h"
#include"Utility/ImGuiSupport.h"
#include"PrimitiveDrawer.h"
#include<imgui.h>
#include<vector>
class RailCamera {
public:
	void Initialize(const ViewProjection& view,const Vector3& position, const Vector3& rotation);
	void Update();
	void Draw();
	 
	
	const ViewProjection& GetViewProjection() const { return viewProjection_; }
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	
private:
	void DrawRailLine();
	void ImGui();
	//ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	//レールカメラの移動曲線
	//スプライン曲線制御点(通過点)
	std::vector<Vector3> controlPoints_;
	// スプライン曲線の頂点リスト
	std::vector<Vector3> pointDrawing;
	// 線分の数
	// 減らすとカクカク、増やすとなめらか
	const size_t segmentCount = 100;
	//カメラのレール移動
	int eyePoint=0;
	int targetPoint = 1;
	int forwardPoint = 2;
	float t = 0;
	Vector3 eyePosition = {0, 0, 0};
	Vector3 targetPosition = {0, 0, 0};
};