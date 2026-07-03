#include "SphereCollider.h"

SphereCollider::SphereCollider()
	: radius(30.0f)
{
}
SphereCollider::~SphereCollider()
{
}
ColliderType SphereCollider::GetColliderType() const
{
    return ColliderType::Sphere;
}

void SphereCollider::SetRadius(float setRadius)
{
    radius = setRadius;
}

float SphereCollider::GetRadius() const
{
    return radius;
}

void SphereCollider::DrawDebug() const
{
    if (isActive == false)
        return;
    int color = GetColor(0, 255, 255);
    if (tag == ColliderTag::Player)
        color = GetColor(0, 255, 0);
    else if (tag == ColliderTag::Enemy)
        color = GetColor(255, 0, 0);
    else if (tag == ColliderTag::PlayerAttack)
        color = GetColor(255, 255, 0);
    DrawSphere3D(position,radius,16,color,color,FALSE);
}
