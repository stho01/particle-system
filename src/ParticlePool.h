//
// Created by stenm on 12.04.2022.
//

#ifndef IMP_PARTICLEPOOL_H
#define IMP_PARTICLEPOOL_H

#include <vector>
#include <queue>
#include <map>
#include "Particle.h"

class ParticlePool {
public:
    ParticlePool();
    Particle& GetParticle();

private:
    std::vector<Particle> _particles;
    std::queue<unsigned int> _deadParticlesIndices;

};

#endif //IMP_PARTICLEPOOL_H
