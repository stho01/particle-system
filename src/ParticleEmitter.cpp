//
// Created by stenm on 12.04.2022.
//

#include "ParticleSystem.h"
#include "GUI.h"

ParticleSystem::ParticleSystem(glm::vec2 position) : _position(position) {}

// ===========================================================

void ParticleSystem::update(double deltaTime) {

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
    }
}

// ===========================================================

void ParticleSystem::draw(SDL_Renderer* renderer) {
    for (const auto& particle: _particles) {
        if (!particle.isAlive) {
            continue;
        }

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
}


Particle& ParticleSystem::getParticle() {
    if (_deadParticlesIndices.empty()) {
        _particles.push_back({});
        return _particles[_particles.size() - 1];
    } else {
        int index = _deadParticlesIndices.front();
        _deadParticlesIndices.pop();
        return _particles[index];
    }
}