#include "RailCamera.h"

void RailCamera::Initialize(const ViewProjection& view, const Vector3& position,const Vector3&rotation) { 

	viewProjection_.Initialize();
	viewProjection_ = view;
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;
	worldTransform_.scale_ = {1,1,1};
	
	controlPoints_ = {
	    {0,  0,  0 },
        {10, 0,  10},
        {20, 10, 20},
        {30, 20, 30},
        {40, 30, 30},
        {50, 40, 20},
	    {60, 30, 10},
        {70, 20, 0 },
        {60, 10, 0 },
        {10, 0,  0 },
        {0,  0,  0 },
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
	/*float transform[Vector3D] = {
	    viewProjection_.translation_.x, viewProjection_.translation_.y,
	    viewProjection_.translation_.z
	};
	float rotate[Vector3D] = {
	    viewProjection_.rotation_.x, viewProjection_.rotation_.y, viewProjection_.rotation_.z};*/
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
