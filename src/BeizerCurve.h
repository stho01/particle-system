//
// Created by stenm on 13.04.2022.
//

#ifndef IMP_BEIZERCURVE_H
#define IMP_BEIZERCURVE_H

#include <glm/glm.hpp>
#include <vector>
#include <array>

struct CubicBeizerCurve {
    glm::vec2 p0;
    glm::vec2 p1;
    glm::vec2 p2;
    glm::vec2 p3;

    explicit operator std::array<glm::vec2*, 4>() { return { &p0, &p1, &p2, &p3, }; }
    explicit operator glm::vec2* (){ return &p0; }
    glm::vec2& operator [](int index) {
        switch (index) {
            case 0: return p0;
            case 1: return p1;
            case 2: return p2;
            case 3: return p3;
            default: throw "Out of bound";
        }
    }

    const int& size() const {
        static const int size = 4;
        return size;
    }
};

class BeizerCurve {

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

    static glm::vec2  cubic(float t, const CubicBeizerCurve& curve) {
        return cubic(t, curve.p0, curve.p1, curve.p2, curve.p3);
    }

    static float cubicProgression(float t, glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) {
        return cubic(t, p0, p1, p2, p3).y;
    }

    static float cubicProgression(float t, const CubicBeizerCurve& curve) {
        return cubic(t, curve.p0, curve.p1, curve.p2, curve.p3).y;
    }

    static glm::vec2 cubicNormalized(float t, glm::vec2 p1, glm::vec2 p2) {
        return cubic(t, {0, 0}, p1, p2, {1, 1});
    }

    template<int size>
    static std::array<glm::vec2, size> getPoints(const CubicBeizerCurve& curve, float modifier = 1.0f) {
        std::array<glm::vec2, size> points;
        for (int i = 0; i < size; i++) {
            points[i] = modifier * cubic((i/(float)size), curve);
        }
        return points;
    }

    template<int size = 100>
    static std::array<float, size> getProgressionValues(const CubicBeizerCurve& curve, float modifier = 1.0f) {
        std::array<float, size> progression;
        for (int i = 0; i < size; i++) {
            auto result = modifier * cubic((i/(float)size), curve);
            progression[i] = result.y;
        }
        return progression;
    }


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
};

#endif //IMP_BEIZERCURVE_H
