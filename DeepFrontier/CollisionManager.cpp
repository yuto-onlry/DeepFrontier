#include "CollisionManager.h"
#include "SphereCollider.h"

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
    {
        return;
    }

    if (collider->IsActive() == false)
    {
        return;
    }

    colliders.push_back(collider);
}

void CollisionManager::CheckAllCollision()
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
    {
        if (collider != nullptr)
        {
            collider->DrawDebug();
        }
    }
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