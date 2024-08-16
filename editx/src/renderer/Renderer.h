#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "../buffer/lexer/Lexer.h"
#include <unordered_map>

class Renderer
{
public:
	Renderer(SDL_Renderer* renderer, TTF_Font* font);
	~Renderer();

	void renderText(const std::string& text, SDL_Color textColor, int x, int y);
    void renderHighlightedText(const std::vector<Token>& tokens, int x, int y);

    std::unordered_map<TokenType, SDL_Color> colorMap = {
        {NUMBER, {60,176,173, 255}},        // Yellow
        {IDENTIFIER, {255, 255, 255, 255}},  // White
        {EQUALS, {255, 165, 0, 255}},        // Orange
        {PARENTHESES, {0, 255, 255, 255}},   // Cyan
        {BINARYOPERATOR, {255, 0, 0, 255}},  // Red
        {LET, {183,209,141, 255}},             // Green
        {CONDITION, {191,157,222, 255}},      // Indigo
        //{STRING_LITERAL, {255, 20, 147, 255}}, // Deep Pink
        //{COMMENT, {128, 128, 128, 255}},     // Gray
        //{ERROR, {255, 0, 0, 255}}            // Red (for errors)
    };


private:
	SDL_Renderer* renderer;
	TTF_Font* font;
};
