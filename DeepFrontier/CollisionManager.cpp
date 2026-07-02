#include "CollisionManager.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include <algorithm>
#include <cmath>

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
    Clear();
}

void CollisionManager::Clear()
{
    colliders.clear();
    collisionHit.clear();
}

void CollisionManager::AddCollider(ColliderBase* collider)
{
    if (collider == nullptr)
        return;

    if (collider->IsActive() == false)
        return;

    if (std::find(colliders.begin(), colliders.end(), collider) != colliders.end())
        return;

    colliders.push_back(collider);
}
void CollisionManager::CheckCollision()
{
    collisionHit.clear();

    for (int i = 0; i < (int)colliders.size(); i++)
    {
        for (int j = i + 1; j < (int)colliders.size(); j++)
        {
            ColliderBase* a = colliders[i];
            ColliderBase* b = colliders[j];
            if (CheckCollision(a, b) == true)
            {
                CollisionHit hit;
                hit.colliderA = a;
                hit.colliderB = b;

                collisionHit.push_back(hit);
            }
        }
    }
}

const std::vector<CollisionHit>& CollisionManager::GetCollisionHit() const
{
    return collisionHit;
}

void CollisionManager::DrawDebug() const
{
    for (ColliderBase* collider : colliders)
        if (collider != nullptr)
            collider->DrawDebug();
}

bool CollisionManager::CheckCollision(ColliderBase* a, ColliderBase* b)
{
    if (a == nullptr || b == nullptr)
    {
        return false;
    }

    if (a->IsActive() == false || b->IsActive() == false)
    {
        return false;
    }

    if (a->GetOwner() != nullptr && a->GetOwner() == b->GetOwner())
    {
        return false;
    }

    if (a->GetColliderType() == ColliderType::Sphere &&
        b->GetColliderType() == ColliderType::Sphere)
    {
        return CheckSphereSphere(a, b);
    }

    if (a->GetColliderType() == ColliderType::Capsule &&
        b->GetColliderType() == ColliderType::Capsule)
    {
        return CheckCapsuleCapsule(a, b);
    }
    if (a->GetColliderType() == ColliderType::Sphere &&
        b->GetColliderType() == ColliderType::Capsule)
    {
        return CheckSphereCapsule(a, b);
    }

    // Enemyカプセル × 攻撃Sphere
    if (a->GetColliderType() == ColliderType::Capsule &&
        b->GetColliderType() == ColliderType::Sphere)
    {
        return CheckSphereCapsule(b, a);
    }

    return false;
}
bool CollisionManager::CheckSphereSphere(ColliderBase* a, ColliderBase* b)
{
    SphereCollider* sphereA = static_cast<SphereCollider*>(a);
    SphereCollider* sphereB = static_cast<SphereCollider*>(b);

    VECTOR posA = sphereA->GetPosition();
    VECTOR posB = sphereB->GetPosition();

    float dx = posA.x - posB.x;
    float dy = posA.y - posB.y;
    float dz = posA.z - posB.z;

    float distanceSq = dx * dx + dy * dy + dz * dz;

    float radius = sphereA->GetRadius() + sphereB->GetRadius();
    float radiusSq = radius * radius;

    return distanceSq <= radiusSq;
}
bool CollisionManager::CheckCapsuleCapsule(ColliderBase* a, ColliderBase* b)
{
    CapsuleCollider* capA = static_cast<CapsuleCollider*>(a);
    CapsuleCollider* capB = static_cast<CapsuleCollider*>(b);

    VECTOR aTop = capA->GetTopCenter();
    VECTOR aBottom = capA->GetBottomCenter();

    VECTOR bTop = capB->GetTopCenter();
    VECTOR bBottom = capB->GetBottomCenter();

    // XZ平面の距離
    float dx = capA->GetPosition().x - capB->GetPosition().x;
    float dz = capA->GetPosition().z - capB->GetPosition().z;

    float xzDistance = dx * dx + dz * dz;

    // Y方向の距離
    float aMinY = aBottom.y;
    float aMaxY = aTop.y;
    float bMinY = bBottom.y;
    float bMaxY = bTop.y;

    float yDistance = 0.0f;

    if (aMaxY < bMinY)
    {
        yDistance = bMinY - aMaxY;
    }
    else if (bMaxY < aMinY)
    {
        yDistance = aMinY - bMaxY;
    }
    else
    {
        yDistance = 0.0f;
    }

    float distanceSq = xzDistance + yDistance * yDistance;

    float radius = capA->GetRadius() + capB->GetRadius();
    float radiusSq = radius * radius;

    return distanceSq <= radiusSq;
}
bool CollisionManager::CheckSphereCapsule(ColliderBase* sphere, ColliderBase* capsule)
{
    SphereCollider* sphereCol = static_cast<SphereCollider*>(sphere);
    CapsuleCollider* capsuleCol = static_cast<CapsuleCollider*>(capsule);

    VECTOR spherePos = sphereCol->GetPosition();

    VECTOR capsuleTop = capsuleCol->GetTopCenter();
    VECTOR capsuleBottom = capsuleCol->GetBottomCenter();

    // XZ平面の距離
    float dx = spherePos.x - capsuleCol->GetPosition().x;
    float dz = spherePos.z - capsuleCol->GetPosition().z;

    float xzDistanceSq = dx * dx + dz * dz;

    // Y方向の距離
    float minY = capsuleBottom.y;
    float maxY = capsuleTop.y;

    float yDistance = 0.0f;

    if (spherePos.y < minY)
    {
        yDistance = minY - spherePos.y;
    }
    else if (spherePos.y > maxY)
    {
        yDistance = spherePos.y - maxY;
    }
    else
    {
        yDistance = 0.0f;
    }

    float distanceSq = xzDistanceSq + yDistance * yDistance;

    float radius = sphereCol->GetRadius() + capsuleCol->GetRadius();
    float radiusSq = radius * radius;

    return distanceSq <= radiusSq;
}