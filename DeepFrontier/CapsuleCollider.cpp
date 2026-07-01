#include "CapsuleCollider.h"

CapsuleCollider::CapsuleCollider()
    :radius(30.0f), 
    height(120.0f)
{
}

CapsuleCollider::~CapsuleCollider()
{
}

ColliderType CapsuleCollider::GetColliderType() const
{
    return ColliderType::Capsule;
}

void CapsuleCollider::SetRadius(float r)
{
    radius = r;
}

float CapsuleCollider::GetRadius() const
{
    return radius;
}

void CapsuleCollider::SetHeight(float h)
{
    height = h;
}

float CapsuleCollider::GetHeight() const
{
    return height;
}

VECTOR CapsuleCollider::GetTopCenter() const
{
    return VGet(position.x, position.y + (height * 0.5f - radius), position.z);
}


VECTOR CapsuleCollider::GetBottomCenter() const
{
    return VGet(position.x, position.y - (height * 0.5f - radius), position.z);
}

void CapsuleCollider::DrawDebug() const
{
    int color = GetColor(0, 255, 255);
    if (isActive == false)
        return;
    if (tag == ColliderTag::Player)
        color = GetColor(0, 255, 0);
    else if (tag == ColliderTag::Enemy)
        color = GetColor(0, 255, 255);
    else if (tag == ColliderTag::PlayerAttack)
        color = GetColor(255, 255, 0);

    VECTOR top = GetTopCenter();
    VECTOR bottom = GetBottomCenter();

    // カプセルコライダーデバッグ表示
    DrawCapsule3D(bottom,top,radius,16,color,color,FALSE);
}