#include "Editor.h"
#include "filebrowser/FileSystem.h"
#include "config.h"

#include <iostream>
#include <algorithm>

Editor::Editor() : window(nullptr), renderer(nullptr), font(nullptr), rendererObj(nullptr), inputHandler(nullptr), fileBrowser(nullptr), isBrowsingFiles(false), selectedIndex(0) {
    init();
}

Editor::~Editor()
{
    cleanup();
}

void Editor::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    window = SDL_CreateWindow("editx", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }

    font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 14);
    if (!font) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    fileBrowser = std::make_shared<FileBrowser>(font, 0, 0, 200, 600);
    fileBrowser->setDirectory(".");

    rendererObj = new Renderer(renderer, font);
    inputHandler = new InputHandler(&pieceTable);
    guiManager = new GUIManager();

    guiManager->addWidget(fileBrowser);
}

void Editor::cleanup() {
    delete inputHandler;
    delete rendererObj;
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Editor::handleFileOperations(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_f && SDL_GetModState() & KMOD_CTRL)) {
        isBrowsingFiles = !isBrowsingFiles;
        fileBrowser->setDirectory(".");
        this->setDirectory(getDirectory());
    }
}

void Editor::renderCursor(const std::string& text)
{
    int cursorPos = inputHandler->getCursorPosition();
    std::string textBeforeCursor = text.substr(0, cursorPos);

    int cursorLine = std::count(textBeforeCursor.begin(), textBeforeCursor.end(), '\n');
    //int cursorLine = inputHandler->getCursorRow();

    int lastNewLinePos = textBeforeCursor.find_last_of('\n');
    int cursorColumn = (lastNewLinePos == std::string::npos) ? cursorPos : cursorPos - lastNewLinePos - 1;

    int cursorX = 270;  // offset
    int textWidth = 0;

    
    std::string textLine = text.substr(lastNewLinePos + 1, cursorPos - lastNewLinePos - 1);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textLine.c_str(), { 0, 0, 0 });
    if (textSurface) {
        textWidth = textSurface->w;
        SDL_FreeSurface(textSurface);
    }

    cursorX += textWidth;

    int lineHeight = TTF_FontHeight(font);

    int cursorY = 10 + cursorLine * lineHeight;

    // render cursor
    SDL_Color cursorColor = { 200, 200, 200, 255 };
    SDL_Surface* cursorSurface = TTF_RenderText_Solid(font, "|", cursorColor);
    SDL_Texture* cursorTexture = SDL_CreateTextureFromSurface(renderer, cursorSurface);
    SDL_Rect cursorRect = { cursorX, cursorY, cursorSurface->w, cursorSurface->h };
    SDL_FreeSurface(cursorSurface);
    SDL_RenderCopy(renderer, cursorTexture, NULL, &cursorRect);
    SDL_DestroyTexture(cursorTexture);
}

std::string processTabs(const std::string& text, int tabSize = 4) {
    std::string processedText;
    for (char c : text) {
        if (c == '\t') {
            // Add spaces equivalent to the tab size
            processedText.append(tabSize, '    ');
        }
        else {
            processedText += c;
        }
    }
    return processedText;
}

void renderLineNumbers(SDL_Renderer* renderer, const std::string& text, TTF_Font* font)
{
    int lineCount = 1 + std::count(text.begin(), text.end(), '\n');
    SDL_Color lineNumberColor = { 68,80,101,255 };

    int lineHeight = TTF_FontHeight(font);

    for (int i = 0; i < lineCount; i++)
    {
        std::string lineNumber = std::to_string(i + 1);
        SDL_Surface* lSurface = TTF_RenderText_Solid(font, lineNumber.c_str(), lineNumberColor);
        SDL_Texture* lTexture = SDL_CreateTextureFromSurface(renderer, lSurface);
        SDL_Rect lineNumberRect = { 250, 10 + i * lineHeight, lSurface->w, lSurface->h };
        SDL_FreeSurface(lSurface);
        SDL_RenderCopy(renderer, lTexture, NULL, &lineNumberRect);
        SDL_DestroyTexture(lTexture);
    }
}

void Editor::run() {
    bool running = true;
    SDL_Event e;

    const int lineNumberWidth = 50;
    pieceTable.init("");
    lexer::initReservedIdentifier();
    std::vector<Token> tokens;

    while (running) {
        SDL_Color bgColor = Config::getInstance().primaryBackgroundColor;
        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderClear(renderer);

        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
                running = false;

            guiManager->handleEvents(e);

            if (isBrowsingFiles)
                handleBrowserNavigation(e);
            else
                inputHandler->handleEvents(e);

            handleFileOperations(e);
        }

        std::string text = pieceTable.getText();
        tokens = lexer::tokenize(text);

        renderLineNumbers(renderer, text, font);
        renderCursor(text);

        rendererObj->renderHighlightedText(tokens, 270, 10);
        guiManager->draw(renderer);

        SDL_RenderPresent(renderer);
    }
}