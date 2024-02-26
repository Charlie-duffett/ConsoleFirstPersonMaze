#pragma once
class Level;

class Cursor {
private:
	int m_x = 0;
	int m_y = 0;

	static constexpr wchar_t cursorChar = '_';
	static constexpr char arrowInput = 224;
	static constexpr char leftArrow = 75;
	static constexpr char rightArrow = 77;
	static constexpr char upArrow = 72;
	static constexpr char downArrow = 80;
	static constexpr char escapeKey = 27;
	static constexpr char spaceKey = 32;
	static constexpr wchar_t wallChar = 219;
	static constexpr wchar_t exitChar = 'X';

	char getColourInput();
public:
	wchar_t getChar() {
		return cursorChar;
	}
	int getX() {
		return m_x;
	}
	int getY() {
		return m_y;
	}
	bool updateCursorPosition(Level& level);
	void placeDoor(wchar_t* level, int index);
	void placeKey(wchar_t* level, int index);
};