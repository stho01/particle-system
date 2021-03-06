#pragma once

#include "GameBase.h"
#include <glm/glm.hpp>
#include <vector>
#include <queue>
#include "ParticleEmitter.h"
#include "Particle.h"

class Game : public GameBase
{
public:
    Game();
    ~Game();

protected:
    bool loadMedias() override;
    bool initialize() override;
    void handleEvent(const SDL_Event* e) override;
    void update(double deltaTime) override;
    void draw(SDL_Renderer* renderer, double deltaTime) override;
    static float randomFloat();

private:
    SDL_Texture* _particleTexture = nullptr;
    bool _isDebug = true;
    ParticleEmitter _emitter;
    int _mouseX;
    int _mouseY;
    int _mouseButton;
};

