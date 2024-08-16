#pragma once
#include <SDL2/SDL.h>
#include "../buffer/TextBuffer.hpp"
#include "../buffer/piece/PieceBuffer.hpp"

class InputHandler
{
public:
	InputHandler(PieceTable* pTable);
	void handleEvents(SDL_Event& e);
	int getCursorPosition() const;
	int getCursorRow() const;
	int findCursorPositionInRow(int row) const;

private:
	TextBuffer* buffer;
	PieceTable* pTable;
	int cursorPosition;
	int cursorRow;
};