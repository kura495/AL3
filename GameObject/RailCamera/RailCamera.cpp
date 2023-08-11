#include "RailCamera.h"

void RailCamera::Initialize(const ViewProjection& view, const Vector3& position,const Vector3&rotation) { 

	viewProjection_.Initialize();
	viewProjection_ = view;
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;
	worldTransform_.scale_ = {1,1,1};
	
	controlPoints_ = {
	    {10, -10, 30},
        {20, 0,   20},
        {30, 10,  30},
        {40, 20,  30},
        {50, 30,  20},
	    {60, 30,  10},
        {70, 20,  0 },
        {60, 10,  0 },
        {10, 0,   0 },
        {0,  0,   0 },
	};
	
	// 線分の数+1個分の頂点座標を計算
	for (size_t i = 0; i < segmentCount + 1; i++) {
		float t = 1.0f / segmentCount * i;
		Vector3 pos = /*Catmull - Rom */ CatMull_Rom(controlPoints_, t);
		// 描画用頂点リストに追加
		pointDrawing.push_back(pos);
	}
}

void RailCamera::Update() { 
	worldTransform_.translation_.z += 0.05f;
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("translation", &worldTransform_.translation_.x, -50.0f, 50.0f);
	ImGui::SliderFloat3("rotation", &worldTransform_.rotation_.x, 0.0f, 70.0f);
	ImGui::End();
}

void RailCamera::Draw() { 
	DrawRailLine();
}

void RailCamera::DrawRailLine() {
	// 3Dラインを描画する
	for (int i = 0; i < segmentCount - 1; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		    pointDrawing[i], pointDrawing[i + 1],/*赤色*/{1.0f, 0.0f, 0.0f, 1.0f});
	}
}
