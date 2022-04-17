//
// Created by stenm on 12.04.2022.
//

#include <iostream>
#include "ParticleEmitter.h"
#include "GUI.h"
#include "BeizerCurve.h"

// ===========================================================

void ParticleEmitter::update(double deltaTime) {
    float timeSinceLastEmition = (SDL_GetTicks() - _lastEmitionTime);
    _emitionRatePrSec = ( 1.0f / _system->rateOverTime) * 1000;

    if (_state != FINISHED && timeSinceLastEmition >= _emitionRatePrSec) {
        _lastEmitionTime = SDL_GetTicks();

        int numberOfParticlesToEmit = (int)timeSinceLastEmition / (int)_emitionRatePrSec;
        for (int i = 0; i < numberOfParticlesToEmit; i++) {
            emit();
        }
    }

    if (_system->looping == false && SDL_GetTicks() - _systemStartTime >= _system->duration) {
        _state = FINISHED;
    }

    // Update particles
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

        particle.velocity += glm::vec2(0, _system->gravityModifier) * (float)deltaTime;
        particle.position += particle.velocity * (float)deltaTime;

        if (_system->sizeOverTime.enabled) {
            particle.size = _system->startSize * BeizerCurve::cubicProgression((float)agePercentage, _system->sizeOverTime.curve);
        }
        if (_system->rotationOverTime.enabled) {
            particle.rotation = _system->startRotation + (360.0f * BeizerCurve::cubicProgression((float)agePercentage, _system->rotationOverTime.curve));
        }

        if (_system->colorOverTime.enabled) {
            auto progression = 1 - BeizerCurve::cubicProgression((float)agePercentage, _system->colorOverTime.curve);
            particle.color.r = (_system->colorOverTime.targetColor.r - _system->startColor.r) * progression + _system->startColor.r;
            particle.color.g = (_system->colorOverTime.targetColor.g - _system->startColor.g) * progression + _system->startColor.g;
            particle.color.b = (_system->colorOverTime.targetColor.b - _system->startColor.b) * progression + _system->startColor.b;
            particle.color.a = (_system->colorOverTime.targetColor.a - _system->startColor.a) * progression + _system->startColor.a;
        }

        if (_system->velocityOverTime.enabled) {
            glm::vec2 velocityProgression = {
                BeizerCurve::cubicProgression((float)agePercentage, _system->velocityOverTime.xCurve),
                BeizerCurve::cubicProgression((float)agePercentage, _system->velocityOverTime.yCurve)
            };

            particle.velocity += (velocityProgression * _system->velocityOverTime.force * (float)deltaTime);
        }

        // particle.color.a = _system->startColor.a * (1-agePercentage);
    }
}

// ===========================================================

void ParticleEmitter::draw(SDL_Renderer* renderer) {
    for (const auto& particle: _particles) {
        if (!particle.isAlive) {
            continue;
        }

        if (_texture == nullptr) {
            drawRect(renderer, particle);
        } else {
            drawTexture(renderer, particle);
        }
    }
}

// ===========================================================

void ParticleEmitter::drawRect(SDL_Renderer* renderer, const Particle& particle) {
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

// ===========================================================

void ParticleEmitter::drawTexture(SDL_Renderer* renderer, const Particle& particle) {
    SDL_Rect dst = {
            (int)particle.position.x,
            (int)particle.position.y,
            particle.size,
            particle.size
    };

    SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
    // SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_MUL);
    SDL_SetTextureColorMod(_texture,
       particle.color.r,
       particle.color.g,
       particle.color.b);

    SDL_SetTextureAlphaMod(
        _texture,
        particle.color.a);

//    SDL_SetRenderDrawColor(
//            renderer,
//            particle.color.r,
//            particle.color.g,
//            particle.color.b,
//            particle.color.a);

    SDL_RenderCopyEx(
        renderer,
        _texture,
        &_textureSrc,
        &dst,
        particle.rotation,
        nullptr,
        SDL_FLIP_NONE);
}

// ===========================================================

void ParticleEmitter::useSystem(ParticleSystem* system) {
    _system = system;

}

// ===========================================================

void ParticleEmitter::useTexture(SDL_Texture* texture) {
    _texture = texture;
    if (_texture != nullptr) {
        SDL_QueryTexture(_texture, nullptr, nullptr, &_textureSrc.w, &_textureSrc.h);
    }
}

// ===========================================================

void ParticleEmitter::emit() {
    Particle* particle = getOrCreateParticle();

    const auto shapeResult = _shape == nullptr ? NO_SHAPE : _shape->calculate();

    particle->isAlive = true;
    particle->startTime = SDL_GetTicks();
    particle->position = _position + shapeResult.positionOffset;
    particle->velocity = shapeResult.directionOfVelocity * _system->startSpeed;
    particle->lifeTime = _system->startLifeTime + ((rand() / (float)RAND_MAX) * _system->startLifeTime * 0.5f); // todo: let system decide lifetime discrepancy
    particle->color = _system->startColor;
    particle->size = _system->startSize;
    particle->rotation = _system->startRotation;
}

// ===========================================================

Particle* ParticleEmitter::getOrCreateParticle() {
    if (_deadParticlesIndices.empty()) {
        _particles.push_back({});
        return &_particles[_particles.size() - 1];
    } else {
        int index = _deadParticlesIndices.front();
        _deadParticlesIndices.pop();
        return &_particles[index];
    }
}

// ===========================================================

void ParticleEmitter::setPosition(glm::vec2 position) {
    _position = position;
}

// ===========================================================

void ParticleEmitter::start() {
    _state = RUNNING;
    _systemStartTime = SDL_GetTicks();
}


