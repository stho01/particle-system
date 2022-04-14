#include "Game.h"
#include "GUI.h"
#include <cmath>
#include <iostream>
#include "BeizerCurve.h"


Game::Game()
    : GameBase("particles") {}

Game::~Game() {
    SDL_DestroyTexture(_particleTexture);
}

// ===========================================================

bool Game::loadMedias()
{
    return  true;
}

// ===========================================================

bool Game::initialize() {
    auto* renderer = getRenderer();
    _particleTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1, 1);
    SDL_SetRenderTarget(renderer, _particleTexture);
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderDrawPoint(renderer,0,0);
    SDL_SetRenderTarget(renderer, nullptr);

    ParticleSystem* system = GUI::GetParticleSystem();
    system->sizeOverTime.p0 = { 0.00f, 0.00f };
    system->sizeOverTime.p1 = { 0.30f, 1.00f };
    system->sizeOverTime.p2 = { 0.70f, 1.00f };
    system->sizeOverTime.p3 = { 1.00f, 0.00f };
    system->rotationOverTime.p0 = { 0.00f, 0.00f };
    system->rotationOverTime.p1 = { 0.50f, 1.0f };
    system->rotationOverTime.p2 = { 1.00f, 0.00f };
    system->rotationOverTime.p3 = { 1.00f, 0.00f };

    _emitter.useSystem(system);
    _emitter.useEmitterShape<ConeEmitterShape>(50, 15);
    _emitter.useTexture(_particleTexture);
    _emitter.setPosition({400, SCREEN_HEIGHT- SCREEN_HEIGHT/3});
    _emitter.start();

    _curve = {
        {0,0},
        {.5,0},
        {.5,1},
        {1,1}
    };


    return true;
}

// ===========================================================

void Game::handleEvent(const SDL_Event* e)
{
    // event handling goes here.
    if (e->type == SDL_KEYDOWN) {
        auto symbol = e->key.keysym.sym;
        if (symbol == SDLK_F1) _isDebug = !_isDebug;

        switch (symbol) {
            case SDLK_1: _selected = 0; break;
            case SDLK_2: _selected = 1; break;
            case SDLK_3: _selected = 2; break;
            case SDLK_4: _selected = 3; break;
        }
    }
}

// ===========================================================

void Game::update(double deltaTime) {
    _mouseButton = SDL_GetMouseState(&_mouseX, &_mouseY);
//    if (button & SDL_BUTTON(SDL_BUTTON_LEFT) && !GUI::HasFocus()) {
//        spawnParticlesRadial(GUI::GetSpawnRate(), (float) _mouseX, (float) _mouseY);
//    }

    _emitter.update(deltaTime);
}

// ===========================================================


void Game::draw(SDL_Renderer* renderer, double deltaTime)
{
    /*
    const glm::vec2 scale(240, 100);
    const glm::vec2 translate(100, 400);

    // try select new point:
    if (_mouseButton & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        glm::vec2 mouse = glm::vec2(_mouseX, _mouseY);
        glm::vec2 newPoint = (mouse - translate) / scale;

        for (int i = 0; i < 4; i++) {
            if (glm::length(newPoint - _curve[i]) <= .1) {
                _curve[i] = newPoint;
                _selected = i;
                break;
            }
        }
    }

    // render controls
    std::array<glm::vec2*, 4> adjustmentPoints = (std::array<glm::vec2*, 4>)_curve;
    const int size = 8;
    for (int i = 0; i < adjustmentPoints.size(); i++) {
        if (i % 2 != 0) {
            SDL_SetRenderDrawColor(renderer, 80,80,80,255);
            auto p1 = (*adjustmentPoints[i-1] * scale + translate);
            auto p2 = (*adjustmentPoints[i] * scale + translate);
            SDL_RenderDrawLineF(renderer, p1.x,p1.y, p2.x,p2.y);
        }

        if (i == _selected)
            SDL_SetRenderDrawColor(renderer, 255,0,0,255);
        else
            SDL_SetRenderDrawColor(renderer, 0,255,0,255);

        auto p = (*adjustmentPoints[i] * scale + translate) - size/2.0f;
        SDL_Rect rect = { (int)p.x, (int)p.y, size, size };
        SDL_RenderFillRect(renderer, &rect);
    }

    // render curve
    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    auto points = BeizerCurve::getPoints<100>(_curve);
    for (int i = 1; i < points.size(); i++) {
        auto p1 = points[i-1] * scale + translate;
        auto p2 = points[i] * scale + translate;
        SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
    }
     */

    _emitter.draw(renderer);

    if (_isDebug) {
        GUI::Show(
            deltaTime,
            _emitter.getNumberOfParticles(),
            _emitter.getNumberOfSleepingParticles());
    }
}

float Game::randomFloat() {
    return ((float)rand() / (float)RAND_MAX);
}
