//
// Created by stenm on 12.04.2022.
//

#include "ParticlePool.h"
#include "Particle.h"

ParticlePool::ParticlePool() = default;

// ===========================================================

Particle& ParticlePool::GetParticle() {
    if (_deadParticlesIndices.empty()) {
        Particle p = {};
        _particles.push_back(p);
        unsigned int lastIndex = _particles.size() - 1;

        return _particles[lastIndex];
    } else {
        auto index = _deadParticlesIndices.front();
        _deadParticlesIndices.pop();

        return _particles[index];
    }
}

// ===========================================================

