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
#include "ParticleSystem.h"
#include "BeizerCurve.h"

class GUI {
private:
    static bool _hasFocus;
    static ParticleSystem _particleSystem;

public:


    static void Show(double deltaTime, int particlesInPool, int deadParticles) {
        _hasFocus = false;
        Begin();

        // ImGui::ShowDemoWindow();

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

        ImGui::SetNextWindowPos(ImVec2(800, 0), ImGuiCond_Always, ImVec2(0,0));
        ImGui::SetNextWindowSize(ImVec2(400, 800));

        if (ImGui::Begin("Particle System", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
            ImGui::InputInt("Duration", &_particleSystem.duration);
            ImGui::Checkbox("Looping", &_particleSystem.looping);
            ImGui::InputFloat("Gravity Modifier", &_particleSystem.gravityModifier);
            ImGui::SliderInt("Rate Over Time", &_particleSystem.rateOverTime, 1, 1000);

            ImGui::Separator();

            ImGui::SliderFloat("Start Speed", &_particleSystem.startSpeed, 0.0f, 5000.0f);
            ImGui::SliderInt("Start Size", &_particleSystem.startSize, 1, 1000);
            ImGui::SliderAngle("Start Rotation", &_particleSystem.startRotation);
            ImGui::InputInt("Start Life Time", &_particleSystem.startLifeTime);

            static ImVec4 color = ImVec4(_particleSystem.startColor.r / 255.0f, _particleSystem.startColor.g / 255.0f, _particleSystem.startColor.b / 255.0f, _particleSystem.startColor.a / 255.0f);
            ImGui::ColorEdit4("Start Color", (float*)&color, ImGuiColorEditFlags_AlphaPreview);
            _particleSystem.startColor = { (Uint8)(color.x * 255), (Uint8)(color.y * 255), (Uint8)(color.z * 255), (Uint8)(color.w * 255)};

            ImGui::Separator();

            static int displayCount  = 100;
            struct Funcs {
                static float SizeOverTime(void*, int i) {
                    return _particleSystem.startSize * BeizerCurve::cubicProgression(
                        (i/(float)displayCount),
                        _particleSystem.sizeOverTime.p0,
                        _particleSystem.sizeOverTime.p1,
                        _particleSystem.sizeOverTime.p2,
                        _particleSystem.sizeOverTime.p3);
                }
            };
            ImGui::PlotLines("Size over time", Funcs::SizeOverTime, NULL, displayCount, 0, NULL, 0.0f, _particleSystem.startSize, ImVec2(0, 80));

        }

        _hasFocus |= ImGui::IsAnyItemHovered() || ImGui::IsAnyItemFocused() || ImGui::IsAnyItemActive();
        ImGui::End();

        End();
    }

    static ParticleSystem* GetParticleSystem() { return &_particleSystem; }


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

//    static void PlotCurve(const char* label, ParticleSystem::Curve* curve, float initialValue) {
//        static int displayCount  = 100;
//
//        struct Funcs {
//            static float SizeOverTime(void*, int i) {
//                return initialValue * BeizerCurve::cubicProgression(
//                        (i/(float)displayCount),
//                        curve->p0,
//                        curve->p1,
//                        curve->p2,
//                        curve->p3);
//            }
//        };
//
//        ImGui::PlotLines("Size over time", Funcs::SizeOverTime, NULL, displayCount, 0, NULL, 0.0f, _particleSystem.startSize, ImVec2(0, 80));
//    }
};

#endif //IMP_GUI_H
