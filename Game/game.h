#pragma once
#include "player.h"
#include "level.h"

class PlaceableActor;
class Game {
private:
	bool m_gameOver;
	Player m_player;
	Level m_level;
public:
	Game() : m_gameOver{ false }, m_player{}, m_level{m_player} 
	{}

	void Completed();
	void Loop();

	bool CanMove(float x, float y, PlaceableActor& actor);
};