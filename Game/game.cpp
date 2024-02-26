#include <Windows.h>
#include <iostream>
#include "game.h"
#include "door.h"
#include "placeableActor.h"
#include "audioManager.h"
#include "renderer.h"


void Game::Loop() {
	Renderer renderer{};
	bool gameActive = true;
	while (gameActive) {
		auto timePassed1 = std::chrono::high_resolution_clock::now();
		auto timePassed2 = std::chrono::high_resolution_clock::now();
		while (!m_gameOver) {
			//Update deltatime at the start of each frame
			timePassed2 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> elapsedTime = timePassed2 - timePassed1;
			timePassed1 = timePassed2;
			float deltaTime = elapsedTime.count();
			//system("cls");
			renderer.Draw(m_player, m_level, deltaTime);
			m_player.Update(*this, deltaTime);
		}
		renderer.ResetConsole();
		std::cout << "congrats you have won the game!" << std::endl;
		std::cout << "Would you like to play another level? (Y/n): ";
		std::string playAgain;
		std::cin >> playAgain;
		if (playAgain == "y" || playAgain == "Y") {
			m_gameOver = false;
			m_level.ChangeLevel(m_player);
		}
		else {
			std::cout << "Thanks for playing";
			gameActive = false;
		}
	}
	AudioManager::destroyInstance();
}


void Game::Completed() {
	AudioManager::getInstance()->playGameWon();
	m_gameOver = true;
}

bool Game::CanMove(float x, float y, PlaceableActor& actor) {
	if (m_level.IsWall(x, y)) {
		return false;
	}
	if (m_level.IsExit(x, y)) {
		Completed();
		return true;
	}
	auto* collidedActor = m_level.CheckActorCollision(x,y);
	if (collidedActor == nullptr) {
		return true; // hasnt collided with anything
	}
	else if(actor == *collidedActor) {
		return true;
	} else if (collidedActor->GetActorType() == ActorType::door) {
		actor.HandleCollision(x,y,*collidedActor); // Run actors handle collision logic

		int collidedX = collidedActor->GetXPosition();
		int collidedY = collidedActor->GetYPosition();
		collidedActor->HandleCollision(collidedX, collidedY, actor); // If actor has key to door it will be set as inactive

		if (collidedActor->IsActive()) { // If door is still active stop actor from moving to that space
			return false;
		}
		return true;
	} else {
		actor.HandleCollision(x,y, *collidedActor); // Run actors handle collision logic

		int collidedX = collidedActor->GetXPosition();
		int collidedY = collidedActor->GetYPosition();
		collidedActor->HandleCollision(collidedX, collidedY, actor); // Run handle collision logic on actor that has been collided with
		return true; // Allow actor to move to space
	}

}