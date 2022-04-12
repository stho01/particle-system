//
// Created by stenm on 12.04.2022.
//

#ifndef IMP_GUI_H
#define IMP_GUI_H

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_sdlrenderer.h>
#include "Color.h"

class GUI {
private:
    static float _gravity[2];
    static int _spawnRate;
    static int _maxParticleSpeed;
    static bool _hasFocus;
    static int _particleLifeTime;
    static int _particleSize;
    static float _particleStartColor[4];
    static float _particleEndColor[4];

public:
    static void Show(double deltaTime, int particlesInPool, int deadParticles) {
        _hasFocus = false;
        Begin();

        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always, ImVec2(0,0));
        ImGui::SetNextWindowBgAlpha(0.9f);
        if (ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav)) {
            ImGui::Text("FPS %i", (int)(1/deltaTime));
            ImGui::Text("Particles in Pool %i", particlesInPool);
            ImGui::Text("Dead Particles %i", deadParticles);
            ImGui::Text(
                    "Map cooridnates (x=%.1f, y=%.1f)",
                    ImGui::GetIO().MousePos.x,
                    ImGui::GetIO().MousePos.y
            );
        }
        _hasFocus |= ImGui::IsAnyItemHovered() || ImGui::IsAnyItemFocused() || ImGui::IsAnyItemActive();
        ImGui::End();

        if (ImGui::Begin("Attributes")) {
            ImGui::InputFloat2("Gravity", _gravity);
            ImGui::SliderInt("Spawn Rate", &_spawnRate, 1, 1000);
            ImGui::Spacing();
            if (ImGui::CollapsingHeader("Particle")) {
                ImGui::InputInt("Max Speed", &_maxParticleSpeed);
                ImGui::SliderInt("Life Time", &_particleLifeTime, 50, 10000);
                ImGui::InputInt("Size", &_particleSize);
                ImGui::Spacing();
                ImGui::ColorPicker4("Start RGBAColor", _particleStartColor);
                ImGui::Spacing();
                ImGui::ColorPicker4("End RGBAColor", _particleEndColor);
            }
        }
        _hasFocus |= ImGui::IsAnyItemHovered() || ImGui::IsAnyItemFocused() || ImGui::IsAnyItemActive();
        ImGui::End();

        End();
    }


    static glm::vec2 GetCurrentGravity() { return {_gravity[0], _gravity[1]}; }
    static int GetParticleMaxSpeed() { return _maxParticleSpeed; }
    static bool HasFocus() { return _hasFocus; }
    static int GetParticleLifeTime() { return _particleLifeTime; }
    static int GetParticleSize() { return _particleSize; }
    static int GetSpawnRate() { return _spawnRate; }

    static RGBAColor GetParticleStartColor() {
        return RGBAColor {
            (Uint8)(_particleStartColor[0] * 255),
            (Uint8)(_particleStartColor[1] * 255),
            (Uint8)(_particleStartColor[2] * 255),
            (Uint8)(_particleStartColor[3] * 255),
        };
    }

    static RGBAColor GetParticleEndColor() {
        return RGBAColor {
                (Uint8)(_particleEndColor[0] * 255),
                (Uint8)(_particleEndColor[1] * 255),
                (Uint8)(_particleEndColor[2] * 255),
                (Uint8)(_particleEndColor[3] * 255),
        };
    }

private:

    static void Begin() {
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }

    static void End() {
        ImGui::Render();
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    }
};

#endif //IMP_GUI_H
