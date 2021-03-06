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
    int startSize = 70;
    float startRotation = 0.0f;
    int startLifeTime = 3000;
    RGBAColor startColor = {255,255,255,5};
    float gravityModifier = 0.0f;
    int rateOverTime = 500;

    struct BeizerCurve {
        bool enabled = false;
        CubicBeizerCurve curve{};
    } sizeOverTime, rotationOverTime;

    struct ColorOverTime {
        bool enabled = false;
        CubicBeizerCurve curve{};
        RGBAColor targetColor = {255,255,255};
    } colorOverTime;

    struct {
        bool enabled = false;
        glm::vec2 force = {10.0f, 10.0f};
        CubicBeizerCurve xCurve{};
        CubicBeizerCurve yCurve{};
    } velocityOverTime;
};

#endif //IMP_PARTICLESYSTEM_H
