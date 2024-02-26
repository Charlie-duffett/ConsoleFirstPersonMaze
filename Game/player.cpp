#include <conio.h>
#include <Windows.h>
#include <cmath>
#include "global.h"
#include "player.h"
#include "game.h"
#include "placeableActor.h"
#include "door.h"
#include "key.h"
#include "Color.h"



void Player::Update(Game& game, float deltaTime) {
	float newPlayerX = m_x;
	float newPlayerY = m_y;

	if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
		m_angle -= (10.0f * 0.75f) * deltaTime;

	// Handle CW Rotation
	if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
		m_angle += (m_speed * 0.75f) * deltaTime;
	// Handle Forwards movement & collision
	if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
	{
		newPlayerX += std::cosf(m_angle) * (m_speed * deltaTime);
		newPlayerY += std::sinf(m_angle) * (m_speed * deltaTime);
	}

	// Handle backwards movement & collision
	if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
	{
		newPlayerX -= std::cosf(m_angle) * (m_speed * deltaTime);
		newPlayerY -= std::sinf(m_angle) * (m_speed * deltaTime);
	}

	
	if (game.CanMove(newPlayerX, m_y, *this)) {
		m_x = newPlayerX;
	}
	if (game.CanMove(m_x, newPlayerY, *this)) {
		m_y = newPlayerY;
	}
	
}

bool Player::HasKey(Color type) {
	switch (type) {
	case Color::red: {
		return (m_redKeys > 0 ? true : false);
		break;
	}
	case Color::green: {
		return (m_greenKeys > 0 ? true : false);
		break;
	}
	case Color::blue: {
		return (m_blueKeys > 0 ? true : false);
		break;
	}
	default: {
		return false;
		break;
	}
	}
}
void Player::AddKey(Color type) {
	switch (type) {
	case Color::red: {
		m_redKeys++;
		break;
	}
	case Color::green: {
		m_greenKeys++;
		break;
	}
	case Color::blue: {
		m_blueKeys++;
		break;
	}
	default: {
		break;
	}
	}
}

void Player::RemoveKey(Color type) {
	switch (type) {
	case Color::red: {
		(m_redKeys > 0 ? --m_redKeys : false);
		break;
	}
	case Color::green: {
		(m_greenKeys > 0 ? --m_greenKeys : false);
		break;
	}
	case Color::blue: {
		(m_blueKeys > 0 ? --m_blueKeys : false);
		break;
	}
	default: {
		break;
	}
	}
}


void Player::HandleCollision(float x, float y, PlaceableActor& collidedActor) {
	Key* isKey = dynamic_cast<Key*>((&collidedActor));
	if (isKey) {
		Color keyType = isKey->GetType();
		AddKey(keyType);
	}
}
