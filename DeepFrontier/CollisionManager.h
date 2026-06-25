#pragma once
#include "ColliderBase.h"
#include <vector>

struct CollisionHit
{
	ColliderBase* colliderA;
	ColliderBase* colliderB;
};

class CollisionManager
{
private:
	std::vector<ColliderBase*> colliders;
	std::vector<CollisionHit> collisionHit;

public:
	CollisionManager();
	~CollisionManager();

public:
	void Clear();
	const std::vector<CollisionHit>& GetCollisionHit() const;
	void AddCollider(ColliderBase* collider);
	void CheckCollision();

	void DrawDebug() const;

private:
	bool CheckCollision(ColliderBase* a, ColliderBase* b);
	bool CheckSphereSphere(ColliderBase* a, ColliderBase* b);
	bool CheckCapsuleCapsule(ColliderBase* a, ColliderBase* b);
	bool CheckSphereCapsule(ColliderBase* sphere, ColliderBase* capsule);
};