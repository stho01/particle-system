//
// Created by stenm on 13.04.2022.
//

#ifndef IMP_BEIZERCURVE_H
#define IMP_BEIZERCURVE_H

#include <glm/glm.hpp>
#include <vector>
#include <array>

class BeizerCurve {

/*
    // Less optimal than the outright calculation...
    // easier to understand tho.

    glm::vec2 beizerCurve(float t, std::vector<glm::vec2> points) {
        if (points.size() == 1) {
            return points[0];
        }

        std::vector<glm::vec2> interpolated;
        for (int i = 1; i < points.size(); i++) {
            auto p0 = points[i-1];
            auto p1 = points[i];
            auto result = (1 - t) * p0 + t * p1;
            interpolated.push_back(result);
        }

        return beizerCurve(t, interpolated);
    }
*/
public:
    static glm::vec2 linear(float t, glm::vec2 p0, glm::vec2 p1) {
        return (1 - t) * p0 + t * p1;
    }

    static glm::vec2 quadric(float t, glm::vec2 p0, glm::vec2 p1, glm::vec2 p2) {
        double tminus = (1-t);
        double a = tminus*tminus;
        double b = 2*tminus*t;
        double c = t*t;

        return {
            a*p0.x + b*p1.x + c*p2.x,
            a*p0.y + b*p1.y + c*p2.y
        };
    }

    static glm::vec2 cubic(float t, glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) {
        double tMinus = (1-t);
        double a = tMinus*tMinus*tMinus;
        double b = 3*t*tMinus*tMinus;
        double c = 3*t*t*tMinus;
        double d = t*t*t;

        return {
            a*p0.x + b*p1.x + c*p2.x + d*p3.x,
            a*p0.y + b*p1.y + c*p2.y + d*p3.y
        };
    }

    static float cubicProgression(float t, glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) {
        return cubic(t, p0, p1, p2, p3).y;
    }

    static glm::vec2 cubicNormalized(float t, glm::vec2 p1, glm::vec2 p2) {
        return cubic(t, {0, 0}, p1, p2, {1, 1});
    }
};

#endif //IMP_BEIZERCURVE_H
