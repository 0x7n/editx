#include "InputHandler.hpp"

InputHandler::InputHandler(PieceTable* pTable) : pTable(pTable)
{
	SDL_StartTextInput();
}

void InputHandler::handleEvents(SDL_Event& e)
{
	std::string currentText = pTable->getText();
	cursorPosition = pTable->getText().length();

	if (e.type == SDL_QUIT) {
		// need to handle this
		exit(0);
	}
	else if (e.type == SDL_TEXTINPUT)
	{
		pTable->insert(cursorPosition, e.text.text);
		cursorPosition += strlen(e.text.text);
	}
	else if (e.type == SDL_KEYDOWN) {
		//buffer->deleteLastCharacter();
		if (e.key.keysym.sym == SDLK_BACKSPACE && cursorPosition > 0)
		{
			pTable->erase(cursorPosition - 1, 1);
			cursorPosition--;
		}
		else if (e.key.keysym.sym == SDLK_UP && cursorRow > 0)
		{
			// move up
			cursorRow--;
			cursorPosition = findCursorPositionInRow(cursorRow);
		}
		else if (e.key.keysym.sym == SDLK_DOWN && cursorRow > 0)
		{
			cursorRow++;
			cursorPosition = findCursorPositionInRow(cursorRow);
		}
		else if (e.key.keysym.sym == SDLK_LEFT && cursorPosition > 0) {
			// Move cursor left
			cursorPosition--;
			if (currentText[cursorPosition] == '\n')
			{
				cursorRow--;
			}
		}
		else if (e.key.keysym.sym == SDLK_RIGHT && cursorPosition < pTable->getText().length()) {
			// Move cursor right
			cursorPosition++;
			if (currentText[cursorPosition] == '\n')
			{
				cursorRow++;
			}
		}
		else if (e.key.keysym.sym == SDLK_RETURN) {
			// Insert newline
			pTable->insert(cursorPosition, "\n");
			cursorPosition++;
		}
	}
}

int InputHandler::getCursorPosition() const
{
	return cursorPosition;
}

int InputHandler::getCursorRow() const
{
	return cursorRow;
}

int InputHandler::findCursorPositionInRow(int row) const
{
	// Calculate the position in the text for a given row.
	int position = 0;
	int currentRow = 0;
	std::string text = pTable->getText();

	for (char ch : text) {
		if (currentRow == row) {
			return position;
		}
		if (ch == '\n') {
			currentRow++;
		}
		position++;
	}
	return position;
}
