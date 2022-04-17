//
// Created by stenm on 13.04.2022.
//

#ifndef IMP_EMITTERSHAPES_H
#define IMP_EMITTERSHAPES_H

#include <glm/glm.hpp>
#include <iostream>


struct EmitterShapeResult {
    glm::vec2 positionOffset;
    glm::vec2 directionOfVelocity;
};

const EmitterShapeResult NO_SHAPE = { glm::vec2(0), glm::vec2(0) };


class IEmitterShape {
public:
    virtual EmitterShapeResult calculate() = 0;
};

class CircleEmitterShape : public IEmitterShape {
public:
    int radius;
    int arc;

    CircleEmitterShape(int radius = 50, int arc = 360) {
        this->radius = radius;
        this->arc = arc;
    }

    EmitterShapeResult calculate() override {
        float angle = (rand() % arc) * M_PI / 180;
        glm::vec2 dir = { cos(angle), sin(angle) };
        glm::vec2 offset = dir * (float)(rand() % radius);

        return { offset, dir };
    };
};

class ConeEmitterShape : public IEmitterShape {
public:
    int radius;
    int height;
    float angle;

    ConeEmitterShape(int radius = 50, float angle = 35, int height = 0) {
        this->radius = radius;
        this->angle = angle;
        this->height = height;
    }

    EmitterShapeResult calculate() override {
        float x = ((float)rand() / (float)RAND_MAX) * (radius - 1.0f);
        float xOffset = x - ((float)radius / 2.0f);
        float fractionOfRadius = x / (float)radius;
        float angleOfVelocity = ((angle * 2.0f * fractionOfRadius) - angle) - 90.0f;
        float angleOfVelocityRad = angleOfVelocity * M_PI / 180.0f;

        glm::vec2 dir = { cos(angleOfVelocityRad), sin(angleOfVelocityRad) };
        glm::vec2 offset = glm::vec2(xOffset, 0) + (dir * (float)(height != 0 ? (rand() % height) : 0));

        return { offset, dir };
    };
};

class DonutEmitterShape : public IEmitterShape {
public:
    int radius;
    float thickness;
    float arc;

    DonutEmitterShape(int radius = 150, float thickness = 20.0f, float arc = 360.0f) {
        this->radius = radius;
        this->thickness = thickness;
        this->arc = arc;
    }

    EmitterShapeResult calculate() override {
        float angle = ((rand() / (float)RAND_MAX) * arc) * M_PI/180;
        float displacement = ((rand() / (float)RAND_MAX) * thickness) - thickness/2;
        glm::vec2 dir = {
            cos(angle),
            sin(angle)
        };
        glm::vec2 offset = dir * (radius + displacement);

        return { offset, dir };
    }
};

#endif //IMP_EMITTERSHAPES_H
