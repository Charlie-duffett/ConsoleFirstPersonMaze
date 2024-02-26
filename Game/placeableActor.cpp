#include <Windows.h>
#include <iostream>
#include "placeableActor.h"

//void PlaceableActor::Draw() {
//	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
//	SetConsoleTextAttribute(console, (int)m_color);
//	std::cout << m_character;
//};

bool operator== (const PlaceableActor& a, const PlaceableActor& b) {
	if (a.GetActorType() == b.GetActorType()) {
		if (a.m_x == b.m_x && a.m_y == b.m_y) {
			return true;
		}
	} 
	return false;
}