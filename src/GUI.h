//
// Created by stenm on 12.04.2022.
//

#ifndef IMP_GUI_H
#define IMP_GUI_H

#include <functional>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_sdlrenderer.h>
#include "Color.h"
#include "ParticleSystem.h"
#include "BeizerCurve.h"
#include "ImGuiPlots.h"

class GUI {
private:
    static bool _showDemo;
    static bool _hasFocus;
    static ParticleSystem _particleSystem;

public:

    static void Show(double deltaTime, int particlesInPool, int deadParticles);
    static ParticleSystem* GetParticleSystem() { return &_particleSystem; }

private:
    static void Begin();
    static void InfoWindow(double deltaTime, int particlesInPool, int deadParticles);
    static void ParticleSystem();
    static void End();
};



#endif //IMP_GUI_H
