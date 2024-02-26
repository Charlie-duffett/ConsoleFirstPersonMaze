#pragma once
#include "level.h"
#include "placeableActor.h"
#include "Color.h"

class Game;
class Player : public PlaceableActor {
private:
	int m_redKeys;
	int m_greenKeys;
	int m_blueKeys;
	int m_health;
	float m_angle;
	float m_speed; 

public:
	Player() : PlaceableActor(0, 0, '@'),
		m_health{ 3 },
		m_redKeys{ 0 },
		m_greenKeys{ 0 },
		m_blueKeys{ 0 },
		m_angle{ 0.0f },
		m_speed{ 5.0f }
	{}
	ActorType GetActorType() const { return ActorType::player; }
	bool HasKey(Color type);
	void AddKey(Color type);
	void RemoveKey(Color type);
	void Update(Game& game, float deltaTime);
	void HandleCollision(float x, float y, PlaceableActor& collidedActor);
	float GetAngle() {
		return m_angle;
	}
};