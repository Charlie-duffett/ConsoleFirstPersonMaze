#include <Windows.h>
#include <conio.h>
#include <iostream>
#include "editor.h"
#include "level.h"
#include "cursor.h"


void Editor::loop() {
	Level level{};
	Cursor cursor{};

	m_isEditing = true;

	while (m_isEditing) {
		system("cls");
		level.displayLevel(cursor);
		m_isEditing = cursor.updateCursorPosition(level);
		//std::cout << _getch() << ", " << _getch();
	}
	system("cls");
	level.displayLevel(cursor);
	level.saveLevel();
}