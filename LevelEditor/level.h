#pragma once
class Cursor;

class Level {
private:
	int m_width;
	int m_height;
	wchar_t* m_level;
	int m_spawnX;
	int m_spawnY;
	static constexpr wchar_t m_keyChar = 232;
	static constexpr wchar_t m_doorChar = 179;
	static constexpr int m_red = 12;
	static constexpr int m_green = 10;
	static constexpr int m_blue = 9;

public:
	Level();
	~Level();
	wchar_t* getLevelArray() {
		return m_level;
	}
	int getWidth() {
		return m_width;
	}
	int getHeight() {
		return m_height;
	}

	void displayLevel(Cursor cursor);
	int getIndexFromXY(int x, int y);
	void saveLevel();
	void updateSpawn(int x, int y);
private:
	void getLevelDimensions();
	void displayWidthBorder();
	void displayHeightBorder();
	void displayControls();
};

