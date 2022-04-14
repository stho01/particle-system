//
// Created by stenm on 13.04.2022.
//

#ifndef IMP_PARTICLESYSTEM_H
#define IMP_PARTICLESYSTEM_H

#include <glm/glm.hpp>
#include "Color.h"
#include "BeizerCurve.h"

struct ParticleSystem {
    int duration = 5000;
    bool looping = true;
    float startSpeed = 50.0f;
    int startSize = 10;
    float startRotation = 0.0f;
    int startLifeTime = 3000;
    RGBAColor startColor = {255,255,255,255};
    float gravityModifier = 0.0f;
    int rateOverTime = 10;

    struct {
        bool enabled = false;
        CubicBeizerCurve curve{};
    } sizeOverTime, rotationOverTime;
};

#endif //IMP_PARTICLESYSTEM_H
