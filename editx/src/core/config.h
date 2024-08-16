#pragma once
#include <SDL2/SDL.h>
#include <string>

struct Config
{
	SDL_Color buttonColor;
	SDL_Color buttonHoverColor;
	SDL_Color textColor;
	SDL_Color primaryBackgroundColor;
	SDL_Color secondaryBackgroundColor;

	std::string fontPath;
	int fontSize;

	static Config& getInstance() {
		static Config instance;
		return instance;
	}

private:
	Config() : 
		buttonColor{100,100,255,255}, 
		buttonHoverColor{ 150,150,255,255 }, 
		textColor{225,225,225,255}, 
		primaryBackgroundColor{ 59, 66, 82, 255 },
		secondaryBackgroundColor{ 46, 52, 64, 255 },
		fontPath("C:\\Windows\\Fonts\\arial.ttf"),
		fontSize(14)
	{}

	Config(const Config&) = delete;
	Config& operator=(const Config&) = delete;
};