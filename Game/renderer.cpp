#include "renderer.h"
#include "player.h"
#include "level.h"
#include "placeableActor.h"
#include <cmath>
#include <vector>
#include <algorithm>
constexpr WORD g_defaultColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

Renderer::Renderer() :
	m_hConsole{ CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL) },
	m_dwBytesWritten{ 0 },
	m_stdConsole{ GetStdHandle(STD_OUTPUT_HANDLE) },
	m_screenHeight{ 0 },
	m_screenWidth{ 0 },
	m_fov{ 3.14159f / 4.0f }, 
	m_renderDistance{ 16.0f }
{
	// Find current size of console window and set height and width vars
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	m_screenHeight = rows;
	m_screenWidth = columns;
	m_colors = new WORD[m_screenWidth * m_screenHeight];
	ResetScreen();
	CreateBuffer();
}
Renderer::~Renderer() {
	delete[] m_screen;
	delete[] m_colors;
	ResetConsole();
}

void Renderer::ResetScreen() {
	if (m_screen != nullptr) {
		delete[] m_screen;
		m_screen = nullptr;
	}
	// create new screen array and fill it with blank spaces
	m_screen = new char[m_screenWidth * m_screenHeight];
	for (int x = 0; x < m_screenWidth; x++) {
		m_colors[x] = g_defaultColor;
		for (int y = 0; y < m_screenHeight; y++) {
			m_screen[x + y * m_screenWidth] = ' ';
		}
	}
}

void Renderer::CreateBuffer() const
{
	SetConsoleActiveScreenBuffer(m_hConsole);
}

void Renderer::ResetConsole() const
{
	SetConsoleActiveScreenBuffer(m_stdConsole);
}

void Renderer::Draw(Player& player, Level& level, float deltaTime) {
	const float playerAngle = player.GetAngle();
	const float playerX = player.GetXPosition();
	const float playerY = player.GetYPosition();
	WORD color = g_defaultColor;
	
	for (int screenX = 0; screenX < m_screenWidth; screenX++) {
		// For each column, calculate the projected ray angle into world space
		// Start from left of fox and then goto right on final X pos
		const float rayAngle = (playerAngle - m_fov / 2.0f) + (static_cast<float>(screenX) / static_cast<float>(m_screenWidth)) * m_fov;

		float distanceToWall = 0.0f; // Starting point

		// calculate the unit vector for the "eye" same as what is done with player movement
		const float eyeX = std::cosf(rayAngle);
		const float eyeY = std::sinf(rayAngle);

		bool objectHit = false;
		bool isObjectEdge = false; // This will be done to put a devider on walls/actor to make the game clearer

		PlaceableActor* actor = nullptr; // Create actor pointer for later.
		int finalX = 0;
		int finalY = 0;

		int lastEyeX = playerX;
		int lastEyeY = playerY;
		while (!objectHit && distanceToWall < m_renderDistance) {
			constexpr float stepSize = 0.1f;
			// Step one further away. Calculate new distanceToWall and current X/Y pos
			distanceToWall += stepSize;
			// We want these values in ints as it'll allow us to directly access the level array easier.
			// We dont care that x is 1.1 we only care about the whole number
			const int currentEyeX = static_cast<int>(playerX + eyeX * distanceToWall);
			const int currentEyeY = static_cast<int>(playerY + eyeY * distanceToWall);
			
			// If the current eye position is out of bounds set the distance to the max render distance
			// then pretend we hit an object
			if (currentEyeX < 0 || currentEyeX >= level.GetWidth() ||
				currentEyeY < 0 || currentEyeY >= level.GetHeight()) {
				objectHit = true;
				distanceToWall = m_renderDistance;
				actor = nullptr; // Make sure the actor is set to nullptr as it may have hit the player first.
				color = g_defaultColor;
			}
			else {
				// We are still inbounds
				if (level.IsWall(currentEyeX, currentEyeY) || level.IsExit(currentEyeX, currentEyeY)) {
					// We have hit an object! currently dont care what we have hit.
					objectHit = true;
					finalX = currentEyeX;
					finalY = currentEyeY;
					isObjectEdge = IsEdge(currentEyeX, currentEyeY, playerX, playerY, eyeX, eyeY);
					color = g_defaultColor;
				}
				else if (currentEyeX != lastEyeX || currentEyeY != lastEyeY) {			
					actor = level.CheckActorCollision(currentEyeX, currentEyeY);
					if (actor) {
						objectHit = true;
						finalX = currentEyeX;
						finalY = currentEyeY;
						isObjectEdge = IsEdge(currentEyeX, currentEyeY, playerX, playerY, eyeX, eyeY);
						color = actor->GetColorAttributes();
					}
				}
			}
			
			lastEyeX = currentEyeX;
			lastEyeY = currentEyeY;
		}
		// Calculate distance to ceiling and floor
		const int nCeiling = static_cast<float>(level.GetHeight() / 2.0f) - level.GetHeight() / distanceToWall;
		const int nFloor = level.GetHeight() - nCeiling;

		char shadedChar = ' '; // This is the char that should be drawn onto screen default if blank.
		if (distanceToWall < m_renderDistance && !isObjectEdge) { // only do this if we have actually hit something
			if (actor == nullptr) {
				// Is either Wall or Exit
				if (level.IsWall(finalX, finalY)) {
					// The object we hit is a wall so set the wall char.
					shadedChar = level.GetWallCharFromDistance(distanceToWall, m_renderDistance);
				}
				else { // its the exit
					shadedChar = 'X'; // this is bad looking for now
				}
			}
			else { // We have hit an actor
				shadedChar = actor->GetCharFromDistance(distanceToWall, m_renderDistance);
			}
		}

		for (int screenY = 0; screenY < m_screenHeight; screenY++)
		{
			const int index = GetScreenIndexFromCoordinates(screenX, screenY);
			// Each Row
			if (screenY <= nCeiling) {
				m_colors[index] = g_defaultColor;
				m_screen[index] = ' ';
			}
			else if (screenY <= nFloor) {
				m_screen[index] = shadedChar;
				m_colors[index] = color;
			}
			else // Floor
			{
				m_colors[index] = g_defaultColor;
				// Shade floor based on distance
				float b = 1.0f - ((static_cast<float>(screenY) - m_screenHeight / 2.0f) / (static_cast<float>(m_screenHeight) / 2.0f));
				if (b < 0.25)		shadedChar = '#';
				else if (b < 0.5)	shadedChar = 'v';
				else if (b < 0.75)	shadedChar = '.';
				else if (b < 0.9)	shadedChar = '-';
				else				shadedChar = ' ';
				m_screen[index] = shadedChar;
			}
		}
	}
	char* map = level.GetArray();
	// Display Map

	const int mapStartX = std::floor(playerX)-5;
	const int mapStartY = std::floor(playerY)-2;
	for (int nx = 0; nx < 11; nx++) {
		for (int ny = 0; ny < 5; ny++)
		{	
			if (mapStartX + nx < 0 || mapStartY + ny < 0 || 
				mapStartX + nx > level.GetWidth() || mapStartY + ny > level.GetHeight()) 
			{
				m_screen[GetScreenIndexFromCoordinates(nx, ny)] = ' ';
			}
			else {
				m_screen[GetScreenIndexFromCoordinates(nx, ny)] = map[level.GetIndexFromCoordinates(mapStartX + nx, mapStartY + ny)];
			}
		}
	}
	m_screen[GetScreenIndexFromCoordinates(5,2)] = 'P';

	// Display Frame
	sprintf_s(m_screen, 50, "X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", playerX, playerY, playerAngle, 1.0f / deltaTime); // debug stuff
	// set the colours then output to screen
	//PopulateColorArray();
	WriteConsoleOutputAttribute(m_hConsole, m_colors, m_screenWidth * m_screenHeight, { 0,0 }, &m_dwBytesWritten);
	WriteConsoleOutputCharacterA(m_hConsole, m_screen, m_screenWidth * m_screenHeight, { 0,0 }, &m_dwBytesWritten);
}

void Renderer::PopulateColorArray() {
	for (int x = 0; x < m_screenWidth; x++) {
		for (int y = 1; y < m_screenHeight; y++) {
			m_colors[GetScreenIndexFromCoordinates(x,y)] = m_colors[x];
		}
	}
}

bool Renderer::IsEdge(const int currentEyeX, const int currentEyeY, const float playerX, const float playerY, const float eyeX, const float eyeY) {
	// To highlight tile boundaries, cast a ray from each corner
						// of the tile, to the player. The more coincident this ray
						// is to the rendering ray, the closer we are to a tile 
						// boundary, which we'll shade to add detail to the walls
	std::vector<std::pair<float, float>> p;

	// Test each corner of hit tile, storing the distance from
	// the player, and the calculated dot product of the two rays
	for (int tx = 0; tx < 2; tx++) {
		for (int ty = 0; ty < 2; ty++)
		{
			// Angle of corner to eye
			float vy = static_cast<float>(currentEyeY) + ty - playerY;
			float vx = static_cast<float>(currentEyeX) + tx - playerX;
			float d = sqrt(vx * vx + vy * vy);
			float dot = (eyeX * vx / d) + (eyeY * vy / d);
			p.push_back(std::make_pair(d, dot));
		}
	}
	// Sort Pairs from closest to farthest
	std::sort(p.begin(), p.end(), [](const std::pair<float, float>& left, const std::pair<float, float>& right) {return left.first < right.first; });

	// First two/three are closest (we will never see all four)
	constexpr float fBound = 0.01f;
	if (std::acos(p.at(0).second) < fBound) {
		return true;
	}
	if (std::acos(p.at(1).second) < fBound) {
		return true;
	}
	/*if (std::acos(p.at(2).second) < fBound) {
		return true;
	}*/
	return false;
}