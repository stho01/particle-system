//
// Created by stenm on 12.04.2022.
//

#include "GUI.h"

ParticleSystem GUI::_particleSystem = {};
bool GUI::_hasFocus = false;
bool GUI::_showDemo = false;

void GUI::Show(double deltaTime, int particlesInPool, int deadParticles) {
    _hasFocus = false;
    Begin();

    if (_showDemo) {
        ImGui::ShowDemoWindow();
    }
    InfoWindow(deltaTime, particlesInPool, deadParticles);
    ParticleSystem();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::Checkbox("Show demo", &_showDemo);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    End();
}

void GUI::Begin() {
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void GUI::InfoWindow(double deltaTime, int particlesInPool, int deadParticles) {
    ImGui::SetNextWindowPos(ImVec2(10, 30), ImGuiCond_Always, ImVec2(0,0));
    ImGui::SetNextWindowBgAlpha(0.9f);
    if (ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav)) {
        ImGui::Text("FPS %i", (int)(1/deltaTime));
        ImGui::Text("Particles in Pool %i", particlesInPool);
        ImGui::Text("Dead Particles %i", deadParticles);
        ImGui::Text("Map cooridnates (x=%.1f, y=%.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
    }
    _hasFocus |= ImGui::IsAnyItemHovered() || ImGui::IsAnyItemFocused() || ImGui::IsAnyItemActive();
    ImGui::End();
}

void GUI::ParticleSystem() {
    ImGui::SetNextWindowPos(ImVec2(800, 18), ImGuiCond_Always, ImVec2(0,0));
    ImGui::SetNextWindowSize(ImVec2(400, 783));
    if (ImGui::Begin("Particle System", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
        ImGui::InputInt("Duration", &_particleSystem.duration);
        ImGui::Checkbox("Looping", &_particleSystem.looping);
        ImGui::InputFloat("Gravity Modifier", &_particleSystem.gravityModifier);
        ImGui::SliderInt("Rate Over Time", &_particleSystem.rateOverTime, 1, 1000);

        ImGui::Separator();

        ImGui::InputFloat("Start Speed", &_particleSystem.startSpeed, 0.0f, 5000.0f);
        ImGui::SliderInt("Start Size", &_particleSystem.startSize, 1, 1000);
        ImGui::SliderAngle("Start Rotation", &_particleSystem.startRotation);
        ImGui::InputInt("Start Life Time", &_particleSystem.startLifeTime);
        ColorEdit("Start Color", &_particleSystem.startColor);
        ImGui::Separator();


        if (ImGui::CollapsingHeader("Size over time")) {
            ImGui::Checkbox("Enable##SizeOverTime", &_particleSystem.sizeOverTime.enabled);
            ImGuiPlots::BeizerCurve("Size over time", _particleSystem.sizeOverTime.curve);
        }
        if (ImGui::CollapsingHeader("Rotation over time")) {
            ImGui::Checkbox("Enable##RotationOverTime", &_particleSystem.rotationOverTime.enabled);
            ImGuiPlots::BeizerCurve("Rotation over time", _particleSystem.rotationOverTime.curve);
        }
        if (ImGui::CollapsingHeader("Color over time")) {
            ImGui::Checkbox("Enable##ColorOverTime", &_particleSystem.colorOverTime.enabled);
            ColorEdit("Target Color", &_particleSystem.colorOverTime.targetColor);
            ImGuiPlots::BeizerCurve("Rotation over time", _particleSystem.colorOverTime.curve);
        }
        if (ImGui::CollapsingHeader("Velocity over time")) {
            ImGui::Checkbox("Enable##ColorOverTime", &_particleSystem.velocityOverTime.enabled);
            ImGui::InputFloat2("Force (x,y)", glm::value_ptr(_particleSystem.velocityOverTime.force));
            ImGuiPlots::BeizerCurve("X Velocity", _particleSystem.velocityOverTime.xCurve);
            ImGuiPlots::BeizerCurve("Y Velocity", _particleSystem.velocityOverTime.yCurve);
        }
    }

    _hasFocus |= ImGui::IsAnyItemHovered() || ImGui::IsAnyItemFocused() || ImGui::IsAnyItemActive();
    ImGui::End();
}

void GUI::End() {
    ImGui::Render();
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}

void GUI::ColorEdit(const char* label, RGBAColor* rgbaColor) {
    ImVec4 color = ImVec4(
            rgbaColor->r / 255.0f,
            rgbaColor->g / 255.0f,
            rgbaColor->b / 255.0f,
            rgbaColor->a / 255.0f);

    ImGui::ColorEdit4(label, (float*)&color, ImGuiColorEditFlags_AlphaPreview);

    rgbaColor->r = (Uint8)(color.x * 255);
    rgbaColor->g = (Uint8)(color.y * 255);
    rgbaColor->b = (Uint8)(color.z * 255);
    rgbaColor->a = (Uint8)(color.w * 255);
}