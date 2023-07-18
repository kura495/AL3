#pragma once
#include"Vector3.h"
class Collider {
public:
	//半径を取得
	float GetRadius() const { return radius_; }
	//半径を設定
	void SetRadius(float& Radius) { radius_ = Radius; }
	//衝突時に呼ばれる関数
	virtual void OnCollision() = 0;
	virtual Vector3 GetWorldPosition()=0;
	uint32_t GetcollitionAttribute() const { return collisionAttribute_; }
	void SetcollitionAttribute(uint32_t collisionAttribute) {
		collisionAttribute_ = collisionAttribute;
	}
	uint32_t GetcollisionMask() const { return collisionMask_; }
	void SetcollisionMask(uint32_t collisionMask){

		collisionMask_ = collisionMask;
	}

private:
	/// <summary>
	/// 半径
	/// 初期値は1.0f
	/// </summary>
	float radius_ = 1.0f;
	/// <summary>
	/// 衝突属性(自分)
	/// </summary>
	uint32_t collisionAttribute_ = 0xffffffff;
	/// <summary>
	/// 衝突マスク(相手)
	/// </summary>
	uint32_t collisionMask_ = 0xffffffff;

};
