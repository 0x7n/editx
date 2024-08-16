
/*

	cheers ChatGPT for helping me with this

*/

#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <functional>
#include <memory>

class Widget
{
protected:
	int x, y, width, height;
	SDL_Color backgroundColor;

public:
	Widget(int x, int y, int width, int height, SDL_Color color) : x(x), y(y), width(width), height(height), backgroundColor(color) {}

	virtual void draw(SDL_Renderer* renderer) = 0;
	virtual void handleEvent(SDL_Event& e) = 0;
};

class Label : public Widget {
private:
	std::string text;

public:
	Label(int x, int y, const std::string& text, SDL_Color color)
		: Widget(x, y, 0, 0, color), text(text) {}

	void draw(SDL_Renderer* renderer) override {
		// doing this shit the same as in the other renderer idc
		TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 14);

		SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), backgroundColor);
		if (!textSurface) {
			return;
		}

		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (!textTexture) {
			SDL_FreeSurface(textSurface);
			return;
		}

		int textWidth = 0;
		int textHeight = 0;
		SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);
		SDL_Rect renderQuad = { x, y, textWidth, textHeight };

		SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);
	}

	void handleEvent(SDL_Event& e) override {}

};

class Button : public Widget {
private:
	Label label;
	std::function<void()> onClick;
	SDL_Color hoverColor;
	bool isHovered;

public:
	Button(int x, int y, int width, int height, SDL_Color color, SDL_Color hoverColor, const std::string& text, SDL_Color textColor, std::function<void()> const& onClick)
		: Widget(x, y, width, height, color), label(x + width / 2 - 50, y + height / 2 - 10, text, textColor), onClick(onClick), hoverColor(hoverColor), isHovered(false) {}


	void draw(SDL_Renderer* renderer) override {
		if (isHovered)
		{
			SDL_SetRenderDrawColor(renderer, hoverColor.r, hoverColor.g, hoverColor.b, hoverColor.a);
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
		}
		SDL_Rect rect = { x,y,width,height };
		SDL_RenderFillRect(renderer, &rect);

		label.draw(renderer);
	}

	void handleEvent(SDL_Event& e) override
	{
		int mouseX = e.button.x;
		int mouseY = e.button.y;

		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			if (isHovered)
			{
				onClick();
			}
		}
		else if (e.type == SDL_MOUSEMOTION)
		{
			isHovered = (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height);
		}
	}

private:

	void renderText(SDL_Renderer* renderer,const std::string& text, int x, int y, SDL_Color color, TTF_Font* font) {
		SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), color);
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

		int textWidth, textHeight;
		TTF_SizeText(font, text.c_str(), &textWidth, &textHeight);
		SDL_Rect textRect = { x - textWidth / 2, y - textHeight / 2, textWidth, textHeight };

		SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

		SDL_DestroyTexture(textTexture);
		SDL_FreeSurface(textSurface);
	}
};


class GUIManager {
private:
	std::vector<std::shared_ptr<Widget>> widgets;

public:
	void addWidget(const std::shared_ptr<Widget>& widget) {
		widgets.push_back(widget);
	}

	void draw(SDL_Renderer* renderer) const {
		for (auto& widget : widgets)
		{
			widget->draw(renderer);
		}
	}

	void handleEvents(SDL_Event& e) const {
		for (auto& widget : widgets)
		{
			widget->handleEvent(e);
		}
	}
};