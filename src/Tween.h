#ifndef IMP_TWEEN_H
#define IMP_TWEEN_H

#include "Color.h"

namespace Utils {
    template<typename T>
    T InterPolate(T start, T end, int lifeTime, int age) {
        T tweenTotal = (end - start);
        float agePercentage = ((float)age / (float)lifeTime);
        T tweenFraction = tweenTotal * agePercentage;

        return (start + tweenFraction);
    }
}

template<typename T>
class Tween {
private:
    T _start;
    T _end;
    int _lifeTime;

public:
    Tween(T start, T end, int lifeTime)
        : _start(start)
        , _end(end)
        , _lifeTime(lifeTime) { }

    T GetValue(int age) {
        return Utils::InterPolate(_start, _end, _lifeTime, age);
    }
};

// ===========================================================

class TweenColor {
private:
    RGBAColor _start;
    RGBAColor _end;
    int _lifeTime;

public:
    TweenColor(RGBAColor start, RGBAColor end, int lifeTime)
        : _start(start), _end(end), _lifeTime(lifeTime) {}

    RGBAColor GetValue(int age) {
        RGBAColor c {};
        c.r = Utils::InterPolate(_start.r, _end.r, _lifeTime, age);
        c.g = Utils::InterPolate(_start.g, _end.g, _lifeTime, age);
        c.b = Utils::InterPolate(_start.b, _end.b, _lifeTime, age);
        c.a = Utils::InterPolate(_start.a, _end.a, _lifeTime, age);
        return c;
    }
};

#endif //IMP_TWEEN_H
