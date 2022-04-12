#include "Game.h"
#include "GUI.h"
#include <cmath>
#include <iostream>


Game::Game() : GameBase("particles") {}
Game::~Game() {}

// ===========================================================

bool Game::loadMedias()
{
    return  true;
}

// ===========================================================

bool Game::initialize() {

    _particles.resize(50000);
    for (int i = 0; i < 50000; i++) {
        _deadParticlesIndices.push(i);
    }

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

void Game::update(double deltaTime)
{
    auto button = SDL_GetMouseState(&_mouseX, &_mouseY);
    if (button & SDL_BUTTON(SDL_BUTTON_LEFT) && !GUI::HasFocus()) {
        SpawnParticlesRadial(GUI::GetSpawnRate(), (float)_mouseX, (float)_mouseY);
    }



    for (int i = 0; i < _particles.size(); i++) {
        auto& particle = _particles[i];
        if (!particle.isAlive) {
            continue;
        }

        unsigned int age = (SDL_GetTicks() - particle.startTime);
        if (age > particle.lifeTime) {
            particle.isAlive = false;
            _deadParticlesIndices.push(i);
            continue;
        }

        double agePercentage = ((double)age / (double)particle.lifeTime);

        particle.velocity += GUI::GetCurrentGravity() * (float)deltaTime;
        particle.position += particle.velocity * (float)deltaTime;

//        float rDiff = (particle.endColor.r - particle.startColor.r) * agePercentage;
//        float gDiff = (particle.endColor.g - particle.startColor.g) * agePercentage;
//        float bDiff = (particle.endColor.b - particle.startColor.b) * agePercentage;
//        particle.color.r = particle.startColor.r + rDiff;
//        particle.color.g = particle.startColor.g + gDiff;
//        particle.color.b = particle.startColor.b + bDiff;

        particle.color.a = (int)(255.0 - (255.0 * agePercentage));

        if (((int)particle.position.y + particle.size) >= SCREEN_HEIGHT) {
            particle.isAlive = false;
            _deadParticlesIndices.push(i);
        }
    }
}

// ===========================================================

void Game::draw(SDL_Renderer* renderer, double deltaTime)
{

    // draw logic goes here.
    for (const auto& particle: _particles) {
        if (!particle.isAlive) {
            continue;
        }

        // draw particle position
        SDL_Rect particleView = {
            (int)particle.position.x,
            (int)particle.position.y,
            particle.size,
            particle.size
        };

        SDL_SetRenderDrawColor(
            renderer,
            particle.color.r,
            particle.color.g,
            particle.color.b,
            particle.color.a);

        SDL_RenderFillRect(renderer, &particleView);
    }

    if (_isDebug) {
        GUI::Show(
            deltaTime,
            _particles.size(),
            _deadParticlesIndices.size());
    }
}

void Game::SpawnParticlesRadial(int count, float x, float y) {
    for (int i = 0; i < count; i++) {
        glm::vec2 position(x,y);
        unsigned int lifeTime = GUI::GetParticleLifeTime();
        unsigned int startTime = SDL_GetTicks();
        RGBAColor startColor = GUI::GetParticleStartColor();
        RGBAColor endColor = GUI::GetParticleEndColor();

        // calculate velocity
        glm::vec2 speed = glm::vec2((rand() % GUI::GetParticleMaxSpeed()));
        float randomAngle = (rand() % 360) * M_PI/180;
        glm::vec2 direction = { cos(randomAngle), sin(randomAngle) };
        glm::vec2 velocity = direction * speed;

        auto& p = GetParticle();
        p.position = position;
        p.velocity = velocity;
        p.isAlive = true;
        p.color = startColor;
//        p.startColor = startColor;
//        p.endColor = endColor;
        p.startTime = startTime;
        p.lifeTime = lifeTime;
        p.size = GUI::GetParticleSize();
    }
}

Particle& Game::GetParticle() {
    if (_deadParticlesIndices.empty()) {
        _particles.push_back({});
        return _particles[_particles.size() - 1];
    } else {
        int index = _deadParticlesIndices.front();
        _deadParticlesIndices.pop();
        return _particles[index];
    }
}

float Game::randomFloat() {
    return ((float)rand() / (float)RAND_MAX);
}
