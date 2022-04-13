#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>

const unsigned int FPS = 244;
const unsigned int MS_PR_FRAME = 1000/FPS;

class GameBase
{
public:
    static const int SCREEN_WIDTH = 1200;
    static const int SCREEN_HEIGHT = 800;

public:
    ~GameBase();

    int run();

protected:
    explicit GameBase(const std::string title) : _title(title) { }

    virtual bool loadMedias() = 0;
    virtual bool initialize() = 0;
    virtual void handleEvent(const SDL_Event* e) = 0;
    virtual void update(double deltaTime) = 0;
    virtual void draw(SDL_Renderer* renderer, double deltaTime) = 0;

    SDL_Texture* loadTexture(const std::string& path);
    SDL_Renderer* getRenderer() const { return _renderer; }

    bool initSdl();

private:
    void pollEvents(SDL_Event* e);
    static bool initImg();
    static bool initImGUI(SDL_Window* window, SDL_Renderer* renderer);
    bool createWindow();

private:
    std::vector<SDL_Texture*> _textures;
    std::string _title = nullptr;
    SDL_Window* _window = nullptr;
    SDL_Renderer* _renderer = nullptr;
    bool _quit = false;
    unsigned int _timeSincePreviousFrame;
    double _deltaTime;
};