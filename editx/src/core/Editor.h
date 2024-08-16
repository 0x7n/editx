#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../buffer/TextBuffer.h"
#include "../renderer/Renderer.h"
#include "../handlers/InputHandler.h"
#include "filebrowser/FileBrowser.h"
#include "../ui/ui.hpp"

#include <fstream>
#include <filesystem>
#include <sstream>
#include <iostream>

class Editor
{
public:
	Editor();
	~Editor();

	void run();
	//void handleNavigation(SDL_Event& e);

	std::string readFileContent(const std::string& filePath) {
		std::ifstream file(filePath);
		if (!file) {
			std::cerr << "Failed to open file: " << filePath << std::endl;
			return "";
		}

		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	void loadFile(const std::string& filePath)
	{
		std::string fileContent = readFileContent(filePath);
		pieceTable.init(fileContent);
	}

	void setDirectory(const std::string& directory) {
		this->currentDirectory = directory;
		items = FileSystem::listFiles(directory);
	}

	std::string getSelectedPath() const {
		if (selectedIndex >= 0 && selectedIndex < items.size()) {
			return currentDirectory + "/" + items[selectedIndex].name;
		}
		return "";
	}

	std::string getRawPath() const
	{
		if (selectedIndex >= 0 && selectedIndex < items.size() && !items[selectedIndex].isDirectory) {
			return std::filesystem::absolute(items[selectedIndex].path).string();
		}
		return ".";
	}

	std::string getDirectory() const
	{
		return currentDirectory;
	}

	void handleBrowserNavigation(SDL_Event const& event)
	{
		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_UP:
			{
				if (selectedIndex > 0) selectedIndex--;
				break;
			}
			case SDLK_DOWN:
			{
				if (selectedIndex < items.size() - 1) selectedIndex++;
				break;
			}
			case SDLK_BACKSPACE:
			{
				//std::string parentDir = std::filesystem::path(currentDirectory).parent_path().string();
				//printf("%s \n", parentDir);
				//setDirectory(parentDir);
				setDirectory(std::filesystem::path(currentDirectory).parent_path().string());
				break;
			}

			case SDLK_RETURN:
			{
				std::string selectedPath = getSelectedPath();
				std::filesystem::path path(selectedPath);
				std::cout << selectedPath << std::endl;
				//std::cout << std::filesystem::absolute(getRawPath()) << std::endl;
				if (!selectedPath.empty()) {
					if (std::filesystem::is_regular_file(path)) {
						//loadFile(getRawPath());
						loadFile(getRawPath());
						std::cout << getRawPath() << std::endl;
					}
					else if(std::filesystem::is_directory(path)){
						setDirectory(selectedPath);
						fileBrowser->setDirectory(selectedPath);
					}
				}
				break;
			}

			default:
				break;
			}
		}
	}

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;
	TextBuffer textBuffer;
	PieceTable pieceTable;
	Renderer* rendererObj;
	InputHandler* inputHandler;
	std::shared_ptr<FileBrowser> fileBrowser;
	GUIManager* guiManager;

	std::vector<FileSystem::FileInfo> items;
	std::string currentDirectory;

	bool isBrowsingFiles;

	int selectedIndex;
	int cursorLine = 0;

	void init();
	void cleanup();
	void handleFileOperations(SDL_Event& e);
	void renderCursor(const std::string& text);
};