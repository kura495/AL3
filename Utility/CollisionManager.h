#pragma once
#include "Utility/Collider.h"
#include "Utility/CollisionConfig.h"
#include <list>
class CollisionManager {
public:
	void AddCollider(Collider* collider) { colliders_.push_back(collider); }
	void ClearCollider() { colliders_.clear(); }
	void CheckAllCollisions();
	/// <summary>
	/// フィルターなし特殊判定用
	/// </summary>
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	void CheckCollision(Collider* colliderA, Collider* colliderB);

private:
	/// <summary>
	/// コライダー登録用
	/// </summary>
	std::list<Collider*> colliders_;

	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
};
