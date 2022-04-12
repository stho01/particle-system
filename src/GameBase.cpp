#include "GameBase.h"
#include <SDL2/SDL_image.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_sdlrenderer.h>

GameBase::~GameBase()
{
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    while (!_textures.empty()) {
        auto it = _textures.begin();
        _textures.erase(it);
        SDL_DestroyTexture(*it);
    }

    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    _window = nullptr;

    IMG_Quit();
    SDL_Quit();
    printf("SDL Quit");
}

int GameBase::run()
{
    if (!initSdl())
    {
        printf("Failed to init SDL");
        return -1;
    }

    if (!loadMedias())
    {
        printf("Failed to load medias");
        return -2;
    }

    if (!initImGUI(_window, _renderer)) {
        printf("Failed to init ImGUI");
        return -3;
    }

    initialize();

    SDL_Event e;
    while(!_quit)
    {
        pollEvents(&e);

        int timeToWait = MS_PR_FRAME - (SDL_GetTicks() - _timeSincePreviousFrame);
        if (timeToWait > 0 && timeToWait <= MS_PR_FRAME) {
            SDL_Delay(timeToWait);
        }

        auto ticks = SDL_GetTicks();
        _deltaTime = (ticks - _timeSincePreviousFrame) / 1000.0;
        _timeSincePreviousFrame = ticks;

        update(_deltaTime);

        SDL_SetRenderDrawColor(_renderer, 22,22,22, 255);
        SDL_RenderClear(_renderer);
        draw(_renderer, _deltaTime);
        SDL_RenderPresent(_renderer);
    }

    return 0;
}

bool GameBase::initSdl()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to init SDL! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    if (!createWindow())
        return false;

    if (!initImg())
        return false;

    return true;
}

void GameBase::pollEvents(SDL_Event* e)
{
    while (SDL_PollEvent(e) != 0)
    {
        ImGui_ImplSDL2_ProcessEvent(e);

        _quit = e->type == SDL_QUIT;
        if (!_quit)
            handleEvent(e);
    }
}

bool GameBase::initImg()
{
    const int imgFlags = IMG_INIT_PNG;

    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_Image could not initialize! SDL_Image error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

bool GameBase::createWindow()
{
    _window = SDL_CreateWindow(
            _title.empty() ? "SDL Lesson" : _title.c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);

    if (_window == nullptr)
    {
        printf("Unable to create window! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

    return true;
}

SDL_Texture* GameBase::loadTexture(const std::string& path) {
    auto texture = IMG_LoadTexture(_renderer, path.c_str());

    if (texture != nullptr) {
        _textures.push_back(texture);
        return texture;
    }

    printf("Failed to load texture: %s", path.c_str());
    return nullptr;
}

bool GameBase::initImGUI(SDL_Window* window, SDL_Renderer* renderer) {

    // init ImGui Context & SDL ImGui Backend.
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);
    return true;
}
