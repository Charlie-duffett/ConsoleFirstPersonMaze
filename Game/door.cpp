#include "placeableActor.h"
#include "door.h"
#include "player.h"
#include "audioManager.h"

void Door::HandleCollision(float x, float y, PlaceableActor& collidedActor) {
	Player* isPlayer = dynamic_cast<Player*>((&collidedActor));
	if (isPlayer) {
		if (isPlayer->HasKey(m_type)) {
			isPlayer->RemoveKey(m_type);
			AudioManager::getInstance()->playDoorOpen();
			m_isActive = false;
		}
		else {
			AudioManager::getInstance()->playDoorClosed();
		}
	}
}

char Door::GetCharFromDistance(float distance, float renderDistance) {
	if (distance <= renderDistance / 4.0f) {
		return Global::wallChar;
	}
	else if (distance < renderDistance / 3.0f) {
		return 178;
	}
	else if (distance < renderDistance / 2.0f) {
		return 177;
	}
	else {
		return 176;
	}
}