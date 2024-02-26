#include "placeableActor.h"
#include "key.h"
#include "player.h"
#include "audioManager.h"

void Key::HandleCollision(float x, float y, PlaceableActor& collidedActor) {
	Player* isPlayer = dynamic_cast<Player*>((&collidedActor));
	if (isPlayer) {
		AudioManager::getInstance()->PlayKeyPickup();
		m_isActive = false;
	}
}