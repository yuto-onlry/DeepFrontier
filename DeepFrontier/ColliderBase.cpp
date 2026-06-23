#include "ColliderBase.h"

ColliderBase::ColliderBase()
	: position({ 0, 0, 0 }),
	  tag(ColliderTag::None),
	  owner(nullptr),
	  isActive(true)
{
}
ColliderBase::~ColliderBase()
{
}
void ColliderBase::SetPosition(const VECTOR& pos)
{
	position = pos;
}
VECTOR ColliderBase::GetPosition() const
{
    return position;
}

void ColliderBase::SetTag(ColliderTag setTag)
{
    tag = setTag;
}

ColliderTag ColliderBase::GetTag() const
{
    return tag;
}

void ColliderBase::SetOwner(CharacterBase* setOwner)
{
    owner = setOwner;
}

CharacterBase* ColliderBase::GetOwner() const
{
    return owner;
}

void ColliderBase::SetActive(bool setActive)
{
    isActive = setActive;
}

bool ColliderBase::IsActive() const
{
    return isActive;
}