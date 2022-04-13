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

//    ParticleSystem system {};
//
//    system.duration = 100;
//    system.gravityModifier = 0;
//    system.rateOverTime = 25;
//    system.looping = true;
//
//    system.startLifeTime = 5000;
//    system.startSize = 40;
//    system.startSpeed = 90;
//    system.startColor = { 255,0,0,255 };

    ParticleSystem* system = GUI::GetParticleSystem();

//    system->sizeOverTime.p0 = { 0.00f, 0.00f };
//    system->sizeOverTime.p1 = { 0.30f, 2.50f };
//    system->sizeOverTime.p2 = { 1.00f, 0.00f };
//    system->sizeOverTime.p3 = { 1.00f, 0.00f };

    system->sizeOverTime.p0 = { 0.00f, 0.00f };
    system->sizeOverTime.p1 = { 0.30f, 2.00f };
    system->sizeOverTime.p2 = { 1.00f, 0.00f };
    system->sizeOverTime.p3 = { 1.00f, 0.00f };

    system->rotationOverTime.p0 = { 0.00f, 0.00f };
    system->rotationOverTime.p1 = { 0.50f, 1.50f };
    system->rotationOverTime.p2 = { 1.00f, 0.00f };
    system->rotationOverTime.p3 = { 1.00f, 0.00f };

    _emitter.useSystem(system);
    _emitter.useEmitterShape<ConeEmitterShape>(50, 15);
    _emitter.useTexture(_particleTexture);
    _emitter.setPosition({400, SCREEN_HEIGHT- SCREEN_HEIGHT/3});
    _emitter.start();

    return true;
}

// ===========================================================

void Game::handleEvent(const SDL_Event* e)
{
    // event handling goes here.
    if (e->type == SDL_KEYDOWN) {
        auto symbol = e->key.keysym.sym;
        if (symbol == SDLK_F1) _isDebug = !_isDebug;
    }
}

// ===========================================================

void Game::update(double deltaTime) {
//    auto button = SDL_GetMouseState(&_mouseX, &_mouseY);
//    if (button & SDL_BUTTON(SDL_BUTTON_LEFT) && !GUI::HasFocus()) {
//        spawnParticlesRadial(GUI::GetSpawnRate(), (float) _mouseX, (float) _mouseY);
//    }

    _emitter.update(deltaTime);
}

// ===========================================================


void Game::draw(SDL_Renderer* renderer, double deltaTime)
{
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
