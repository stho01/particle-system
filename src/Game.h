#pragma once

#include "GameBase.h"
#include <glm/glm.hpp>
#include <vector>
#include <queue>
#include "Particle.h"
#include "ParticlePool.h"

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

    void SpawnParticlesRadial(int count, float x, float y);
    static float randomFloat();

    Particle& GetParticle();



private:
    SDL_Texture* _pngImage = nullptr;
    bool _isDebug = true;
    std::vector<Particle> _particles;
    std::queue<unsigned int> _deadParticlesIndices;
    int _mouseX;
    int _mouseY;
};

