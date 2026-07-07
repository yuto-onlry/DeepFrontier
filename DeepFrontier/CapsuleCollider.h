#pragma once
#include "ColliderBase.h"

class CapsuleCollider : public ColliderBase
{
private:
    float radius;
    float height;

public:
    CapsuleCollider();
    virtual ~CapsuleCollider();

    ColliderType GetColliderType() const override;
public:
    float GetHeight() const;
    float GetRadius() const;
    VECTOR GetPosition() const;
public:
    void SetHeight(float h);
    void SetRadius(float r);

    VECTOR GetTopCenter() const;
    VECTOR GetBottomCenter() const;

    void DrawDebug() const override;
};