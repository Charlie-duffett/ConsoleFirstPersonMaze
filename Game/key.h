#pragma once
#include "placeableActor.h"
#include "global.h"
#include "Color.h"

class Key : public PlaceableActor {
private:
	Color m_type;
public:
	Key(float x, float y, Color color) : PlaceableActor{ x,y, Global::keyChar, color }, m_type{ color } {}
	ActorType GetActorType() const { return ActorType::key; }
	Color GetType() { return m_type; }
	void HandleCollision(float x, float y, PlaceableActor& collidedActor);
};