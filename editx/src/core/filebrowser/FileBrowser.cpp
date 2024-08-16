#include "FileBrowser.h"
#include <iostream>
#include <filesystem>
#include "../config.h"

std::string FileBrowser::getSelectedPath() const {
    if (selectedIndex >= 0 && selectedIndex < items.size()) {
        return currentDirectory + "/" + items[selectedIndex].name;
    }
    return "";
}

std::string FileBrowser::getRawPath() const
{
    if (selectedIndex >= 0 && selectedIndex < items.size() && !items[selectedIndex].isDirectory) {
        return items[selectedIndex].path;
    }
    return ".";
}

std::string FileBrowser::getDirectory() const
{
    return currentDirectory;
}

void FileBrowser::setDirectory(const std::string& directory) {
    this->currentDirectory = directory;
    items = FileSystem::listFiles(directory);
    updateFileButtons();  // update buttons when dir changed
}

void FileBrowser::handleEvent(SDL_Event& event) {
    for (auto const& button : fileButtons) {
        if(button)
            button->handleEvent(event);
    }
}

void FileBrowser::draw(SDL_Renderer* renderer) {
    // render the background
    SDL_Color bgColor = Config::getInstance().secondaryBackgroundColor;
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(renderer, &box);

    // draw buttons
    for (const auto& button : fileButtons) {
        button->draw(renderer);
    }
}

void FileBrowser::updateFileButtons() {
    fileButtons.clear();
    int yOffset = 10;
    int buttonHeight = 30;

    for (size_t i = 0; i < items.size(); i++) {
        std::string name = items[i].name + (items[i].isDirectory ? "/" : "");

        auto onClick = [this, i]() {
            if (items[i].isDirectory) {
                std::string newDir = currentDirectory + "/" + items[i].name;
                setDirectory(newDir);
            }
            else {
                // open file
            }
        };

        // this was fucking painful to type, idk might be a better solution
        auto button = std::make_shared<Button>(box.x + 10, box.y + yOffset, box.w - 20, buttonHeight, Config::getInstance().secondaryBackgroundColor, Config::getInstance().primaryBackgroundColor, name, Config::getInstance().textColor, onClick);
        fileButtons.push_back(button);
        yOffset += buttonHeight;
    }
}