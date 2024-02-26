#pragma once

class AudioManager {
private:
	static AudioManager* s_pInstance;

	AudioManager() {};
public:
	static AudioManager* getInstance() {
		if (s_pInstance == nullptr) {
			s_pInstance = new AudioManager();
		}
		return s_pInstance;
	}

	static void destroyInstance() {
		delete s_pInstance;
		s_pInstance = nullptr;
	}
	void playDoorClosed();
	void playDoorOpen();
	void playGameWon();
	void PlayKeyPickup();
};