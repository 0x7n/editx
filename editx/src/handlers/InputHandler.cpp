#include "InputHandler.h"

InputHandler::InputHandler(PieceTable* pTable) : pTable(pTable)
{
	SDL_StartTextInput();
}

void InputHandler::handleEvents(SDL_Event& e)
{
	std::string currentText = pTable->getText();
	cursorPosition = pTable->getText().length();

	switch (e.type)
	{
		case SDL_QUIT:
		{
			exit(0);
			break;
		}
		case SDL_TEXTINPUT:
		{
			pTable->insert(cursorPosition, e.text.text);
			cursorPosition += strlen(e.text.text);
			break;
		}
		case SDL_KEYDOWN:
		{
			switch(e.key.keysym.sym)
			{
				case SDLK_BACKSPACE:
				{
					if (cursorPosition < 0)
						break;

					pTable->erase(cursorPosition - 1, 1);
					cursorPosition--;
					break;
				}

				case SDLK_UP:
				{
					if (cursorRow < 0)
						break;

					// move up
					cursorRow--;
					cursorPosition = findCursorPositionInRow(cursorRow);
					break;
				}
				case SDLK_DOWN:
				{
					if (cursorRow < 0)
						break;

					cursorRow++;
					cursorPosition = findCursorPositionInRow(cursorRow);
					break;
				}

				case SDLK_LEFT:
				{
					if (cursorPosition < 0)
						break;

					cursorPosition--;
					if (currentText[cursorPosition] == '\n')
					{
						cursorRow--;
					}
					break;
				}

				case SDLK_RIGHT:
				{
					if (cursorPosition > pTable->getText().length())
						break;

					cursorPosition++;
					if (currentText[cursorPosition] == '\n')
					{
						cursorRow++;
					}
					break;
				}

				case SDLK_RETURN:
				{
					pTable->insert(cursorPosition, "\n");
					cursorPosition++;
					break;
				}
			}
			break;
		}

		default:
			break;
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
