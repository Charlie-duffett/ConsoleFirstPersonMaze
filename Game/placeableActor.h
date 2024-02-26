#pragma once
#include "global.h"
#include "Color.h"
#include <Windows.h>

enum class ActorType {
	player,
	door,
	key,
};

class Game;
class PlaceableActor
{
public:
	PlaceableActor(float x, float y, char character, Color color = Color::standard) : m_x{ x }, m_y{ y }, m_color{ color }, m_isActive{ true }, m_character{character} {
		switch (color) {
		case(Color::red): {
			m_colorAttributes = FOREGROUND_RED;
			break;
		}
		case(Color::green): {
			m_colorAttributes = FOREGROUND_GREEN;
			break;
		}
		case(Color::blue): {
			m_colorAttributes = FOREGROUND_BLUE;
			break;
		}
		default: {
			m_colorAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
			break;
		}
		}
	}
	virtual ~PlaceableActor() {};

	float GetXPosition() { return m_x; }
	float GetYPosition() { return m_y; }
	float* GetXPositionPointer() { return &m_x; }
	float* GetYPositionPointer() { return &m_x; }
	void SetPosition(float x, float y) { 
		m_x = x;
		m_y = y;
	}
	char GetChar() {
		return m_character;
	}

	Color GetColor() { return m_color; }
	WORD GetColorAttributes() { return m_colorAttributes; }

	void Remove() { 
		m_isActive = false;
	}
	bool IsActive() { return m_isActive; }

	virtual ActorType GetActorType() const = 0;
	virtual void Draw() {};
	virtual void Update(Game& game)
	{

	}
	// This should be used so actors can control what they do rather than it being in the game/level logic
	virtual void HandleCollision(float x, float y, PlaceableActor& collidedActor)
	{

	}
	virtual char GetCharFromDistance(float distance, float renderDistance)
	{
		return m_character;
	}

	friend bool operator== (const PlaceableActor& a, const PlaceableActor& b);

protected:
	float m_x;
	float m_y;
	char m_character;
	bool m_isActive;
	Color m_color;
	WORD m_colorAttributes;
};