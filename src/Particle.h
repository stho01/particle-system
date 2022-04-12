//
// Created by stenm on 12.04.2022.
//

#ifndef IMP_PARTICLE_H
#define IMP_PARTICLE_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "Color.h"

struct Particle {
    glm::vec2 position;
    glm::vec2 velocity;
    int size;
    bool isAlive;
    RGBAColor color;
    unsigned int startTime;
    unsigned int lifeTime;
};

#endif //IMP_PARTICLE_H
