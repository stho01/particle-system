//
// Created by stenm on 12.04.2022.
//

#ifndef IMP_PARTICLESYSTEM_H
#define IMP_PARTICLESYSTEM_H

#include <SDL2/SDL.h>
#include <vector>
#include <queue>
#include <glm/glm.hpp>
#include <memory>
#include "Particle.h"
#include "Tween.h"

class ParticleSystem {
private:
    glm::vec2 _position;
    std::vector<Particle> _particles;
    std::queue<int> _deadParticlesIndices;
    bool _looping = false;

public:
    explicit ParticleSystem(glm::vec2 position);
    void update(double deltaTime);
    void draw(SDL_Renderer* renderer);

private:
    Particle& getParticle();

};


#endif //IMP_PARTICLESYSTEM_H
