#pragma once
#include <string>
#include <vector>


class PlaceableActor;
class Player;
class Level {
private:
	char* m_level;
	bool m_levelHasWarnings;
	int m_mapWidth;
	int m_mapHeight;
	std::vector<PlaceableActor*> m_pActors;

public:
	Level(Player& player);
	~Level();

	char* GetArray() {
		return m_level;
	}
	int GetWidth() {
		return m_mapWidth;
	}
	int GetHeight() {
		return m_mapHeight;
	}
	int GetIndexFromCoordinates(int x, int y) {
		return x + y * m_mapWidth;
	}
	
	void ChangeLevel(Player& player);
	void Draw();
	void DrawActors();
	void UpdateActors();
	bool IsWall(int x, int y);
	bool IsExit(int x, int y);
	char GetWallCharFromDistance(float distance, float renderDistance);
	PlaceableActor* CheckActorCollision(int x, int y);
private:
	std::string SelectLevel();
	bool LoadLevel(std::string levelName);
	void ValidateLevel(Player& player);
	void Convert(Player& player);
};