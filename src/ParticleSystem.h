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
#include "ParticlePool.h"

class ParticleSystem {
private:
    glm::vec2 _position;
    std::unique_ptr<ParticlePool>& _pool;
    std::vector<Particle> _particles;
    std::queue<int> _deadParticlesIndices;
    TweenColor _color;
    bool _looping;

public:
    explicit ParticleSystem(glm::vec2 position, std::unique_ptr<ParticlePool>& pool);
    void Update(double deltaTime);

};


#endif //IMP_PARTICLESYSTEM_H
