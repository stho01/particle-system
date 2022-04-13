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

        particle.size = _system->startSize * BeizerCurve::cubicProgression(
                (float)agePercentage,
                _system->sizeOverTime.p0,
                _system->sizeOverTime.p1,
                _system->sizeOverTime.p2,
                _system->sizeOverTime.p3);

        particle.rotation = _system->startRotation + (360.0f * BeizerCurve::cubicProgression(
                (float)agePercentage,
                _system->rotationOverTime.p0,
                _system->rotationOverTime.p1,
                _system->rotationOverTime.p2,
                _system->rotationOverTime.p3));

//        float rDiff = (particle.endColor.r - particle.startColor.r) * agePercentage;
//        float gDiff = (particle.endColor.g - particle.startColor.g) * agePercentage;
//        float bDiff = (particle.endColor.b - particle.startColor.b) * agePercentage;
//        particle.color.r = particle.startColor.r + rDiff;
//        particle.color.g = particle.startColor.g + gDiff;
//        particle.color.b = particle.startColor.b + bDiff;

        particle.color = _system->startColor;
        particle.color.a = (int)(255.0 - (255.0 * agePercentage));
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
    particle->lifeTime = _system->startLifeTime;
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


