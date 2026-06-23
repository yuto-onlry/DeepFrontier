#pragma once
#include "ColliderBase.h"

class SphereCollider : public ColliderBase
{
private:
    float radius;

public:
    SphereCollider();
    virtual ~SphereCollider();

public:
	ColliderType GetColliderType() const override;
    void SetRadius(float setRadius);
    float GetRadius() const;

    void DrawDebug() const override;
};