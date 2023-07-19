#include "Skydome.h"

void Skydome::Initialize(Model* model) { 
	model_ = model;

}

void Skydome::Update() {


}

void Skydome::Draw(const ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_);
}
