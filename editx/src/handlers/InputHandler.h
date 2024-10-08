#pragma once
#include <SDL2/SDL.h>
#include "../buffer/TextBuffer.h"
#include "../buffer/piece/PieceBuffer.h"

class InputHandler {
public:
	explicit InputHandler(PieceTable* pTable);
	void handleEvents(SDL_Event& e);
	int getCursorPosition() const;
	int getCursorRow() const;
	int findCursorPositionInRow(int row) const;

private:
	TextBuffer* buffer;
	PieceTable* pTable;
	int			cursorPosition;
	int			cursorRow;
};