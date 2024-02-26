#include <Windows.h>
#include "audioManager.h"


AudioManager* AudioManager::s_pInstance = nullptr;

void AudioManager::playDoorClosed() {
	Beep(500, 75);
	Beep(500, 75);
}
void AudioManager::playDoorOpen() {
	Beep(1397, 200);
}
void AudioManager::playGameWon() {
	Beep(1568, 200);
	Beep(1568, 200);
	Beep(1568, 200);
	Beep(1245, 1000);
	Beep(1397, 200);
	Beep(1397, 200);
	Beep(1397, 200);
	Beep(1175, 1000);
}

void AudioManager::PlayKeyPickup() {
	Beep(1568, 200);
}