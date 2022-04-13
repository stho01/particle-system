//
// Created by stenm on 12.04.2022.
//

#ifndef IMP_PARTICLEEMITTER_H
#define IMP_PARTICLEEMITTER_H

#include <SDL2/SDL.h>
#include <vector>
#include <queue>
#include <glm/glm.hpp>
#include <memory>
#include "Particle.h"
#include "Tween.h"
#include "EmitterShapes.h"
#include "ParticleSystem.h"

class ParticleEmitter {
private:
    enum State {
        PENDING = 0,
        RUNNING = 1,
        FINISHED = 2
    };

    glm::vec2 _position;
    std::vector<Particle> _particles;
    std::queue<unsigned int> _deadParticlesIndices;
    State _state = PENDING;
    ParticleSystem* _system;
    std::unique_ptr<IEmitterShape> _shape;
    unsigned int _systemStartTime;
    unsigned int _lastEmitionTime;
    float _emitionRatePrSec;
    SDL_Texture* _texture;
    SDL_Rect _textureSrc {};

public:
    void start();
    void update(double deltaTime);
    void draw(SDL_Renderer* renderer);
    void setPosition(glm::vec2 position);
    void useSystem(ParticleSystem* system);
    void useTexture(SDL_Texture* texture);

    template<typename T, typename ...TArgs>
    void useEmitterShape(TArgs&& ...args) { _shape = std::make_unique<T>(std::forward<TArgs>(args)...); }

    bool started() { return _state == RUNNING; }
    int getNumberOfParticles() {return _particles.size(); }
    int getNumberOfSleepingParticles() {return _deadParticlesIndices.size(); }

private:
    void emit();
    void drawTexture(SDL_Renderer* renderer, const Particle& particle);
    void drawRect(SDL_Renderer* renderer, const Particle& particle);
    Particle* getOrCreateParticle();

};


#endif //IMP_PARTICLEEMITTER_H
