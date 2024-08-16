#include "Renderer.h"
#include <sstream> 
#include <vector> 
#include <iostream>

Renderer::Renderer(SDL_Renderer* renderer, TTF_Font* font) : renderer(renderer), font(font) {}

Renderer::~Renderer(){}

void Renderer::renderText(const std::string& text, SDL_Color textColor, int x, int y)
{
    std::stringstream ss(text);
    std::string line;
    int lineHeight = TTF_FontHeight(font);  // Get the height of a line of text

    std::vector<std::string> lines;
    while (std::getline(ss, line, '\n')) {
        lines.push_back(line);
    }

    for (size_t i = 0; i < lines.size(); ++i) {
        if (lines[i].empty()) {
            // render a space instead of \n
            lines[i] = " ";
        }

        SDL_Surface* textSurface = TTF_RenderText_Blended(font, lines[i].c_str(), textColor);
        if (!textSurface) {
            std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
            continue;
        }

        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (!textTexture) {
            std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(textSurface);
            continue;
        }

        int textWidth = 0, textHeight = 0;
        SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);
        SDL_Rect renderQuad = { x, y + static_cast<int>(i) * lineHeight, textWidth, textHeight };

        SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
}

void Renderer::renderHighlightedText(const std::vector<Token>& tokens, int x, int y)
{
    int lineHeight = TTF_FontHeight(font);
    int tabWidth = 4 * TTF_FontHeight(font);
    int marginRight = 270;

    for (const auto& token : tokens)
    {
        SDL_Color color = colorMap[token.type];
        if (token.type == TokenType::WHITESPACE) {
            int width;
            TTF_SizeText(font, " ", &width, nullptr);
            x += width;
        }
        else if (token.type == TokenType::NEWLINE) {
            x = marginRight;
            y += lineHeight;
        }
        else if (token.type == TokenType::TAB) {
            x += tabWidth;
        }
        else {
            renderText(token.value, color, x, y);
            int width;
            TTF_SizeText(font, token.value.c_str(), &width, nullptr);
            x += width;
        }
    }
}
