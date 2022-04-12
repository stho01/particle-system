//
// Created by stenm on 12.04.2022.
//

#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(glm::vec2 position, std::unique_ptr<ParticlePool>& pool)
    : _position(position), _pool(pool) {}

void ParticleSystem::Update(double deltaTime) {

}
