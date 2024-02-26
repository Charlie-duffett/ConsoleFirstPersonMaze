#include <iostream>
#include <Windows.h>
#include <fstream>
#include <limits>
#include "level.h"
#include "player.h"
#include "global.h"
#include "placeableActor.h"
#include "key.h"
#include "door.h"
#include "Color.h"

Level::Level(Player& player)	
{
	/*
	m_screenHeight = 30;
	m_screenWidth = 120;
	ChangeLevel(player);
	*/
	ChangeLevel(player);	
}

Level::~Level() {
	delete[] m_level;
}

void Level::ChangeLevel(Player& player) {
	if (m_level != nullptr) {
		delete[] m_level;
		m_level = nullptr;
	}
	bool levelLoaded = false;
	while (!levelLoaded) {
		std::string levelName = SelectLevel();
		levelLoaded = LoadLevel(levelName);
	}
	Convert(player);
	ValidateLevel(player);
	if (m_levelHasWarnings) {
		std::cout << "There was some warnings in the level data please see the error message(s) above.\n";
		system("pause");
		m_levelHasWarnings = false;
	}
}

//void Level::Draw() {
//	/*
//	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
//	SetConsoleTextAttribute(console, (int)Color::standard);
//	for (int y = 0; y < m_mapHeight; ++y) {
//		for (int x = 0; x < m_mapWidth; ++x) {
//			const int index = Level::GetIndexFromCoordinates(x, y);
//			std::cout << m_level[index];
//		}
//		std::cout << std::endl;
//	}
//	DrawActors();
//	*/
//	for (int y = 0; y < m_mapHeight; ++y) {
//		for (int x = 0; x < m_mapWidth; ++x) {
//			const int levelIndex = GetIndexFromCoordinates(x, y);
//			const int screenIndex = GetScreenIndexFromCoordinates(x, y);
//			m_screen[screenIndex] = m_level[levelIndex];
//		}
//	}
//	
//	DrawActors();
//	// Display Frame
//	m_screen[m_screenWidth * m_screenHeight - 1] = '\0';
//	WriteConsoleOutputCharacterA(m_hConsole, m_screen, m_screenWidth * m_screenHeight, { 0,0 }, &m_dwBytesWritten);
//}
//
//void Level::DrawActors() {
//	/*HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
//	COORD actorCursorPosition;
//	for (auto actor = m_pActors.begin(); actor < m_pActors.end(); actor++) {
//		if ((*actor)->IsActive()) {
//			actorCursorPosition.X = (*actor)->GetXPosition();
//			actorCursorPosition.Y = (*actor)->GetYPosition();
//			SetConsoleCursorPosition(console, actorCursorPosition);
//			(*actor)->Draw();
//		}
//	}
//	// Set cursor to end of the level
//	actorCursorPosition.X = 0;
//	actorCursorPosition.Y = m_mapHeight;
//	SetConsoleCursorPosition(console, actorCursorPosition);*/
//	WORD defaultColour = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
//
//	for (auto actor = m_pActors.begin(); actor < m_pActors.end(); actor++) {
//		if ((*actor)->IsActive()) {
//			int x = (*actor)->GetXPosition();// implicit converts to int
//			int y = (*actor)->GetYPosition();
//			const int screenIndex = GetScreenIndexFromCoordinates(x, y);
//			m_screen[screenIndex] = (*actor)->GetChar();
//			WORD attributes = (*actor)->GetColorAttributes();
//			if (attributes != NULL) {
//				WriteConsoleOutputAttribute(m_hConsole, &attributes, 1, { (SHORT)x, (SHORT)y }, &m_dwBytesWritten);
//			}
//			if ((*actor)->GetChar() == '@') {
//				Player* isPlayer = dynamic_cast<Player*>(*actor);
//				float xf = (*actor)->GetXPosition();// implicit converts to int
//				float yf = (*actor)->GetYPosition();
//				sprintf_s(m_screen, 50, "A=%3.2f,X=%3.2f,Y=%3.2f", (*isPlayer).GetAngle(),xf, yf); // Display player angle
//			}
//		}
//	}
//}

std::string Level::SelectLevel() {
	std::string levelName;
	std::cout << "Please type in the filename of the level you would like to play (test.txt): ";
	std::cin >> levelName;
	if (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		std::cout << "You entered something invalid! \n";
	}
	return levelName;
}


bool Level::LoadLevel(std::string fileName) {
	fileName.insert(0, "../");
	std::ifstream levelFile;
	levelFile.open(fileName);
	if (!levelFile) {
		std::cout << "Something went wrong opening the file! \n";
		return false;
	}
	constexpr int tempSize = 25;
	char temp[tempSize];
	levelFile.getline(temp, tempSize, '\n');
	m_mapWidth = std::atoi(temp);

	levelFile.getline(temp, tempSize, '\n');
	m_mapHeight = std::atoi(temp);

	m_level = new char[m_mapWidth * m_mapHeight];
	levelFile.read(m_level, m_mapWidth * m_mapHeight);
	return true;
}

//bool Level::LoadLevel(std::string fileName) {
//	
//	fileName.insert(0, "../");
//	std::ifstream levelFile;
//	levelFile.open(fileName);
//	if (!levelFile) {
//		std::cout << "Something went wrong opening the file! \n";
//		return false;
//	}
//	constexpr int tempSize = 25;
//	char temp[tempSize];
//	levelFile.getline(temp, tempSize, '\n');
//	m_mapWidth = std::atoi(temp);
//
//	levelFile.getline(temp, tempSize, '\n');
//	m_mapHeight = std::atoi(temp);
//
//	//char* tempLevel = new char[m_mapWidth * m_mapHeight];
//	m_level = new unsigned char[m_mapWidth * m_mapHeight];
//	levelFile.read((char*)m_level, m_mapWidth * m_mapHeight);
//
//	/*m_mapWidth = 20;
//	m_mapHeight = 20;
//	std::wstring tempLevel = L"█████████████████████@ █    R          ██  █    █          ██  █b   █          ██  ██████          ██  █               ██  █               ██                  ██                  ██           █████████           █ r    ██           █      ██           █      ██           █      ██                  ██ ███████████████████  █        █      ██  G        █      ██  █        B     X█████████████████████";
//	
//	m_level = new char[m_mapWidth * m_mapHeight];
//
//	for (int i = 0; i < m_mapWidth * m_mapHeight - 1; ++i) {
//		m_level[i] = tempLevel[i];
//	}*/
//	return true;	
//}


/*
bool Level::LoadLevel(std::string fileName) {
	fileName.insert(0, "../");
	std::ifstream levelFile;
	levelFile.open(fileName);
	if (!levelFile) {
		std::cout << "Something went wrong opening the file! \n";
		return false;;
	}
	constexpr int tempSize = 25;
	char temp[tempSize];
	levelFile.getline(temp, tempSize, '\n');
	m_mapWidth = std::atoi(temp);

	levelFile.getline(temp, tempSize, '\n');
	m_mapHeight = std::atoi(temp);

	m_level = new char[m_mapWidth * m_mapHeight];
	char* tempLevel = new char[m_mapWidth * m_mapHeight];
	levelFile.read(tempLevel, m_mapWidth * m_mapHeight);
	// convert level from ascii to wchar
	std::mbtowc(nullptr, 0, 0);
	std::mbtowc(m_level, tempLevel, m_mapWidth * m_mapHeight - 1);

	delete[] tempLevel;
	tempLevel = nullptr;
	return true;
}
*/
void Level::ValidateLevel(Player& player) {
	bool isInvalid = false;
	for (int y = 0; y < m_mapHeight; y++) {
		for (int x = 0; x < m_mapWidth; x++) {
			int index = GetIndexFromCoordinates(x, y);
			if (m_level[index] != Global::wallChar &&  m_level[index] != Global::exitChar && m_level[index] != ' ') {
				std::cout << "An invalid value has been found in the level. It has been replaced with a space. num: " << (unsigned int)m_level[index] << "\n";
				isInvalid = true;
			}
		}
	}
	m_levelHasWarnings = isInvalid;
}

void Level::Convert(Player& player) {
	for (int y = 0; y < m_mapHeight; y++) {
		for (int x = 0; x < m_mapWidth; x++) {
			bool makeSpace = true;
			int index = GetIndexFromCoordinates(x, y);
			switch (m_level[index]) {
			case(Global::playerChar): {
				m_level[index] = ' ';
				player.SetPosition(x, y);
				break;
			} 
			case 'r': {
				m_pActors.push_back(new Key(x, y, Color::red));
				break;
			}
			case 'g': {
				m_pActors.push_back(new Key(x, y, Color::green));
				break;
			}
			case 'b': {
				m_pActors.push_back(new Key(x, y, Color::blue));
				break;
			}
			case 'R': {
				m_pActors.push_back(new Door(x, y, Color::red));
				break;
			}
			case 'G': {
				m_pActors.push_back(new Door(x, y, Color::green));
				break;
			}
			case 'B': {
				m_pActors.push_back(new Door(x, y, Color::blue));
				break;
			}
			/*case 219: {
				m_level[index] = '#';
				break;
			}*/
			default: {
				makeSpace = false;
				break;
			}
			}
			if (makeSpace) {
				m_level[index] = ' ';
			}
		}
	}
}

bool Level::IsWall(int x, int y) {
	int index = GetIndexFromCoordinates(x, y);
	if (m_level[index] == Global::wallChar) {
		return true;
	}
	return false;
}

bool Level::IsExit(int x, int y) {
	int index = GetIndexFromCoordinates(x, y);
	if (m_level[index] == Global::exitChar) {
		return true;
	}
	return false;
}

char Level::GetWallCharFromDistance(float distance, float renderDistance) {
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

PlaceableActor* Level::CheckActorCollision(int x, int y) {
	for (auto actor = m_pActors.begin(); actor < m_pActors.end(); actor++) {
		if ((*actor)->IsActive()) {
			int actorX = (*actor)->GetXPosition();
			int actorY = (*actor)->GetYPosition();
			if (actorX == x && actorY == y) {
				return (*actor);
			}
		}
	}
	return nullptr;
}