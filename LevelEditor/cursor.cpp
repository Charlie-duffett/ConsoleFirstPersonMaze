#include <conio.h>
#include <Windows.h>
#include <iostream>

#include "cursor.h"
#include "level.h"

bool Cursor::updateCursorPosition(Level& level) {
	char input = _getch();
	wchar_t* levelArray = level.getLevelArray();

	const int height = level.getHeight();
	const int width = level.getWidth();

	int newCursorX = m_x;
	int newCursorY = m_y;
	int index = level.getIndexFromXY(newCursorX, newCursorY);

	if (input == arrowInput) {
		input = _getch();
		switch (input) {
		case upArrow: {
			--newCursorY;
			break;
		}
		case leftArrow: {
			--newCursorX;
			break;
		}
		case downArrow: {
			++newCursorY;
			break;
		}
		case rightArrow: {
			++newCursorX;
			break;
		} default: {
			break;
		}
		}
		if (newCursorX >= 0 && newCursorX < width) {
			m_x = newCursorX;
		}

		if (newCursorY >= 0 && newCursorY < height) {
			m_y = newCursorY;
		}
	}
	else {
		switch (input) {
		case 'w':
		case 'W': {
			levelArray[index] = wallChar;
			break;
		}
		case 'd':
		case 'D': {
			placeDoor(levelArray, index);
			break;
		}
		case 'k':
		case 'K': {
			placeKey(levelArray, index);
			break;
		}
		case spaceKey: {
			levelArray[index] = ' ';
			break;
		}
		case 'x':
		case 'X': {
			levelArray[index] = exitChar;
			break;
		}
		case escapeKey: {
			return false;
		}
		case '@': {
			level.updateSpawn(newCursorX, newCursorY);
			break;
		}
		default: {
			break;
		}
		}
	}
	return true;
}

char Cursor::getColourInput() {
	bool isValid = true;
	do {
		char colour;
		std::cin >> colour;
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
			std::cout << "You entered something invalid!\n Try again: ";
			isValid = false;
		}
		else {
			switch (colour) {
			case 'r':
			case 'R': {
				return 'R';
				break;
			}
			case 'g':
			case 'G': {
				return 'G';
				break;
			}
			case 'b':
			case 'B': {
				return 'B';
				break;
			}
			default: {
				std::cout << "You entered something invalid!\n  Try again: ";
				isValid = false;
				break;
			}
			};
		}
	} while (!isValid);
}

void Cursor::placeDoor(wchar_t* level, int index) {
	system("cls");
	std::cout << "What colour type would you like the door to be? (RGB): ";
	char doorColour = getColourInput();
	level[index] = doorColour;
}

void Cursor::placeKey(wchar_t* level, int index) {
	system("cls");
	std::cout << "What colour type would you like the door to be? (RGB): ";
	char keyColour = getColourInput();
	level[index] = std::tolower(keyColour);
}