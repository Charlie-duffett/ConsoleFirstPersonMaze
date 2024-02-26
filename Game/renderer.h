#pragma once
#include <Windows.h>
class Player;
class Level;
class Renderer {
private: 
	int m_screenHeight;
	int m_screenWidth;
	char* m_screen;
	HANDLE m_hConsole;
	HANDLE m_stdConsole;
	DWORD m_dwBytesWritten;
	float m_fov;
	float m_renderDistance;
	WORD* m_colors;

public:
	Renderer();
	~Renderer();

	void ResetScreen();

	int GetScreenIndexFromCoordinates(int x, int y) {
		return x + y * m_screenWidth;
	}

	void CreateBuffer() const;
	void ResetConsole() const;

	void Draw(Player& player, Level& level, float deltaTime);
	void PopulateColorArray();
	static bool IsEdge(int currentEyeX, int currentEyeY, float playerX, float playerY, float eyeX, float eyeY);
};