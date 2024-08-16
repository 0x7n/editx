#pragma once
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "FileSystem.hpp"
#include "../../buffer/TextBuffer.hpp"
#include "../../ui/ui.hpp"
#include "../config.hpp"

class FileBrowser : public Widget
{
private:
    TTF_Font* font;
    SDL_Rect box;
    std::vector<std::shared_ptr<Button>> fileButtons;
    int selectedIndex;
    std::string currentDirectory;
    std::vector<FileSystem::FileInfo> items;

public:
    FileBrowser(TTF_Font* font, int x, int y, int width, int height)
        : Widget(x, y, width, height, Config::getInstance().secondaryBackgroundColor),
        font(font), box{ x, y, width, height }, selectedIndex(0) {
        updateFileButtons();
    }

    void draw(SDL_Renderer* renderer) override;
    void handleEvent(SDL_Event& event) override;

	std::string getSelectedPath() const;
	std::string getRawPath() const;
	std::string getDirectory() const;
	void setDirectory(const std::string& directory);

private:
    void updateFileButtons();
};