#pragma once
#include "placeableActor.h"
#include "global.h"
#include "Color.h"

class Door : public PlaceableActor {
private:
	Color m_type;
public:
	Door(float x, float y, Color color) : PlaceableActor{ x,y, Global::doorChar, color }, m_type{ color } {}
	ActorType GetActorType() const { return ActorType::door; }
	Color GetType() { return m_type; }
	void HandleCollision(float x, float y, PlaceableActor& collidedActor);
	char GetCharFromDistance(float distance, float renderDistance);
};