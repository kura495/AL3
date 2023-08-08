#include "RailCamera.h"


void RailCamera::Initialize(ViewProjection viewProjection) { 
	worldTransform_.translation_ = viewProjection.translation_;
	worldTransform_.rotation_ = viewProjection.rotation_;
	worldTransform_.scale_ = {1,1,1};
	viewProjection_.Initialize();
	controlPoints_ = {
	    {0,  0,  0},
        {10, 10, 0},
        {10, 15, 0},
        {20, 15, 0},
        {20, 0,  0},
        {30, 0,  0},
	};
}

void RailCamera::Update() { 
	//worldTransform_.translation_.z += 0.05f;
	worldTransform_.matWorld_ =
	    MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	viewProjection_.Map();
	ImGui::Begin("Camera");
	float transform[Vector3D] = {
	    viewProjection_.translation_.x, viewProjection_.translation_.y,
	    viewProjection_.translation_.z
	};
	float rotate[Vector3D] = {
	    viewProjection_.rotation_.x, viewProjection_.rotation_.y, viewProjection_.rotation_.z};
	ImGui::SliderFloat3("translation", &worldTransform_.translation_.x, -50.0f, 50.0f);
	ImGui::SliderFloat3("rotation", &worldTransform_.rotation_.x, 0.0f, 70.0f);
	ImGui::End();
}

void RailCamera::DrawLine() {
	//線分で描画する用の頂点リスト
	std::vector<Vector3> pointDrawing;
	//線分の数
	const size_t segmentCount = 100;
	//線分の数+1個分の頂点座標を計算
	for (size_t i = 0; i < segmentCount + 1; i++) {
		float t = 1.0f / segmentCount * i;
		Vector3 pos = /*Catmull - Rom */(controlPoints_, t);
		//描画用頂点リストに追加
		pointDrawing.push_back(pos);
	}
}
