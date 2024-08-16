#pragma once
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "FileSystem.h"
#include "../../buffer/TextBuffer.h"
#include "../../ui/ui.h"
#include "../config.h"

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

	std::string_view getSelectedPath() const;
	std::string_view getRawPath() const;
	std::string_view getDirectory() const;
	void setDirectory(const std::string& directory);

private:
    void updateFileButtons();
};