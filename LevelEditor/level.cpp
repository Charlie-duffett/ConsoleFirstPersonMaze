#include <iostream>
#include <Windows.h>
#include <fstream>

#include "level.h"
#include "cursor.h"

Level::Level() {
	getLevelDimensions();
	m_level = new wchar_t[m_width * m_height];
	for (int i = 0; i < m_width * m_height; i++) {
		m_level[i] = ' ';
	}
	//Create map border of walls and set default spawn point
	for (int i = 0; i < 2; i++) {
		for (int x = 0; x < m_width; x++) {
			if (i == 0) {
				int index = getIndexFromXY(x, 0);
				m_level[index] = 219;
			}
			else {
				int index = getIndexFromXY(x, m_height-1);
				m_level[index] = 219;
			}
		}
		for (int y = 0; y < m_height; y++) {
			if (i == 0) {
				int index = getIndexFromXY(0, y);
				m_level[index] = 219;
			}
			else {
				int index = getIndexFromXY(m_width-1, y);
				m_level[index] = 219;
			}
		}
	}
	m_level[getIndexFromXY(1, 1)] = '@';
	m_spawnX = 1;
	m_spawnY = 1;
}

Level::~Level() {
	delete[] m_level;
}

void Level::getLevelDimensions() {
	std::cout << "Enter the width of your level: ";
	std::cin >> m_width;
	std::cout << "Enter the height of your level: ";
	std::cin >> m_height;
}

void Level::displayLevel(Cursor cursor) {
	displayControls();
	displayWidthBorder();
	for (int y = 0; y < m_height; y++) {
		displayHeightBorder();
		for (int x = 0; x < m_width; x++) {
			if (cursor.getY() == y && cursor.getX() == x) {
				std::wcout << cursor.getChar();;
			}
			else {
				HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
				int index = getIndexFromXY(x, y);
				switch (m_level[index]) {
				case 'r': {
					SetConsoleTextAttribute(console, m_red);
					std::wcout << m_keyChar;
					break;
				}
				case 'g': {
					SetConsoleTextAttribute(console, m_green);
					std::wcout << m_keyChar;
					break;
				}
				case 'b': {
					SetConsoleTextAttribute(console, m_blue);
					std::wcout << m_keyChar;
					break;
				}
				case 'R': {
					SetConsoleTextAttribute(console, m_red);
					std::wcout << m_doorChar;
					break;
				}
				case 'G': {
					SetConsoleTextAttribute(console, m_green);
					std::wcout << m_doorChar;
					break;
				}
				case 'B': {
					SetConsoleTextAttribute(console, m_blue);
					std::wcout << m_doorChar;
					break;
				}
				default: {
					SetConsoleTextAttribute(console, 7);
					std::wcout << m_level[index];
					break;
				}
				}
				
			}
		}
		displayHeightBorder();
		std::cout << "\n";
	}
	displayWidthBorder();
}

int Level::getIndexFromXY(int x, int y) {
	return x + y * m_width;
}

void Level::saveLevel() {
	std::cout << "Pick a name for your level file (eg: Level1.txt): ";
	std::string levelName;
	std::cin >> levelName;

	levelName.insert(0, "../");

	std::wofstream levelFile;
	levelFile.open(levelName);
	if (!levelFile) {
		std::cout << "Opps! Something went wrong opening the file.";
	}
	else {
		levelFile << m_width << "\n";
		levelFile << m_height << "\n";
		levelFile.write(m_level, m_width * m_height);
		if (!levelFile) {
			std::cout << "Opps! Something went wrong writing to the file.";
		}
		levelFile.close();
	}

}

void Level::displayWidthBorder() {
	for (int i = 0; i < m_width + 2; i++) {
		std::cout << "+";
	}
	std::cout << "\n";
}
void Level::displayHeightBorder() {
	std::cout << "+";
}

void Level::displayControls() {
	std::cout << "Use arrows to navigate, use W to add walls, use K to add a key, use D to add a door, and use X to add an exit! \nOnce you have designed your level press escape to save and quit.\n";
}

void Level::updateSpawn(int x, int y) {
	int currentIndex = getIndexFromXY(m_spawnX, m_spawnY);
	int newIndex = getIndexFromXY(x, y);

	m_level[currentIndex] = ' ';
	m_level[newIndex] = '@';

	m_spawnX = x;
	m_spawnY = y;
}