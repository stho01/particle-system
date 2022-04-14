//
// Created by stenm on 14.04.2022.
//

#ifndef IMP_IMGUIPLOTS_H
#define IMP_IMGUIPLOTS_H

#define IMGUI_DEFINE_MATH_OPERATORS

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_sdlrenderer.h>
#include <string>
#include <sstream>
#include "BeizerCurve.h"
#include <iostream>
#include <algorithm>

namespace ImGuiPlots {

    // ===========================================================

    struct Domain {
        float xMax = -FLT_EPSILON;
        float xMin = FLT_EPSILON;
        float yMax = -FLT_EPSILON;
        float yMin = FLT_EPSILON;
    };

    // ===========================================================

    template<typename T>
    static T clamp(T min, T max, T value) {
        return fmin(max, fmax(min, value));
    }

    // ===========================================================

    class LinearScale {
    private:
        float _domain[2];
        float _domainDiff;
        float _range[2];
        float _rangeDiff;

    public:
        LinearScale() {
            _domain[0] = 0.0f;
            _domain[1] = 1.0f;
            _domainDiff = 1.0f;
            _range[0] = 0.0f;
            _range[1] = 1.0f;
            _rangeDiff = 1.0f;
        }

        static LinearScale create() {
            return LinearScale();
        }

        LinearScale& domain(float min, float max) {
            _domain[0] = min;
            _domain[1] = max;
            _domainDiff = max - min;
            return *this;
        }

        LinearScale& range(float min, float max) {
            _range[0] = min;
            _range[1] = max;
            _rangeDiff = max - min;
            return *this;
        }

        ImVec2 range() const {
            return { _range[0], _range[1] };
        }
        ImVec2 domain() const {
            return { _domain[0], _domain[1] };
        }

        float get(float value) const {
            float inDomain = (value - _domain[0])/_domainDiff;
            float result = inDomain * _rangeDiff + _range[0];
            return clamp(_range[0], _range[1], result);
        }

        float inverse(float value) const {
            float inRange = (value - _range[0])/_rangeDiff;
            float result = inRange * _domainDiff + _domain[0];
            return clamp(_domain[0], _domain[1], result);
        }
    };

    // ===========================================================

    template<typename T>
    static Domain getDomain(T points) {
        Domain domain {};
        for (int i = 0; i < points.size(); i++) {
            const auto& point = points[i];
            domain.xMax = fmax(domain.xMax, point.x);
            domain.xMin = fmin(domain.xMin, point.x);
            domain.yMax = fmax(domain.yMax, point.y);
            domain.yMin = fmin(domain.yMin, point.y);
        }
        return domain;
    }

    // ===========================================================

    template<typename TPoints>
    static void addCurve(const TPoints& points, const LinearScale& xScale, const LinearScale& yScale) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        const auto baseColor = ImGui::GetColorU32(ImGuiCol_FrameBg);

        auto xRange = xScale.range();
        auto yRange = yScale.range();

        // draw bg.
        drawList->AddRectFilled({xRange.x, yRange.x}, {xRange.y, yRange.y}, baseColor);

        // draw curve
        for (int i = 1; i < points.size(); i++) {
            auto x1 = xScale.get(points[i-1].x);
            auto y1 = yScale.get(1-points[i-1].y);
            auto x2 = xScale.get(points[i].x);
            auto y2 = yScale.get(1-points[i].y);
            drawList->AddLine({x1,y1}, {x2,y2}, IM_COL32(255,255,255,255));
        }
    }

    // ===========================================================

    static void addControls(CubicBeizerCurve& curve, float controlSize, const LinearScale& xScale, const LinearScale& yScale) {
        // RENDER CONTROL POINTS.
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        const auto baseColor = ImGui::GetColorU32(ImGuiCol_SliderGrab);

        for (int i = 0; i < curve.size(); i++) {
            auto x = xScale.get(curve[i].x);
            auto y = yScale.get(1-curve[i].y);

            if (i % 2 != 0) {
                auto x1 = xScale.get(curve[i-1].x);
                auto y1 = yScale.get(1-curve[i-1].y);
                drawList->AddLine({x1,y1}, {x,y}, IM_COL32(80,80,80,200));
            }

            drawList->AddCircleFilled({x,y},controlSize,baseColor);
        }
    }

    // ===========================================================

    template<typename TPoints>
    static void curveEditor(const char* label, CubicBeizerCurve& curve, TPoints& points) {
        const ImGuiContext& g = *GImGui;
        const float minW = 240.0f;
        const float minH = 100.0f;
        const int padding = 2;
        const int controlSize = 6;


        const ImVec2 canvasP0 = ImGui::GetCursorScreenPos();
        ImVec2 canvasSize = ImGui::GetContentRegionAvail();
        canvasSize.x = fmax(canvasSize.x, minW);
        canvasSize.y = fmax(canvasSize.y, minH);
        const ImVec2 canvasP1 = ImVec2(canvasP0.x + canvasSize.x, canvasP0.y + canvasSize.y);

        auto xScale = LinearScale::create().range(canvasP0.x+controlSize, canvasP1.x-controlSize);
        auto yScale = LinearScale::create().range(canvasP0.y+controlSize, canvasP1.y-controlSize);

        // PROCESS INTERACTION
        static glm::vec2* p = nullptr;
        const ImVec2 mousePos = g.IO.MousePos;
        if (g.IO.MouseDown[0])
        {
            if (p == nullptr)
            {
                for (int i = 0; i < 4; i++) {
                    const ImVec2 controlPos = {
                        xScale.get(curve[i].x),
                        yScale.get(1-curve[i].y)
                    };

                    if (ImLengthSqr(mousePos - controlPos) < controlSize*controlSize) {
                        std::cout << "Found point: " << i << std::endl;
                        p = &curve[i];
                        break;
                    }
                }
            }
        }
        else
        {
            p = nullptr;
        }

        if (p != nullptr) {
            if (p != &curve[0] && p != &curve[3]) {
                p->x = xScale.inverse(mousePos.x);
            }

            p->y = (1-yScale.inverse(mousePos.y));
        }

        addCurve(points, xScale, yScale);
        addControls(curve, controlSize, xScale, yScale);

        ImGui::InvisibleButton("CurveEditor", canvasSize);
    }

    // ===========================================================

    static void BeizerCurve(const char* label, CubicBeizerCurve& curve) {
        ImGui::PushID(label);

        //
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImGuiStyle& style = g.Style;

        // define layout
        ImVec2 labelSize = ImGui::CalcTextSize(label, nullptr, true);
        ImVec2 frameSize = { ImGui::CalcItemWidth(), 20 };
        ImRect frameBBox(window->DC.CursorPos, window->DC.CursorPos + frameSize);
        ImRect innerBBox(frameBBox.Min + style.FramePadding, frameBBox.Max - style.FramePadding);

        auto points = BeizerCurve::getPoints<100>(curve);

        auto xScale = LinearScale::create().range(frameBBox.Min.x, frameBBox.Max.x);
        auto yScale = LinearScale::create().range(frameBBox.Min.y, frameBBox.Max.y);

        addCurve(points, xScale, yScale);

        if (labelSize.x > 0.0f)
            ImGui::RenderText(ImVec2(frameBBox.Max.x + style.ItemInnerSpacing.x, innerBBox.Min.y), label);

        if (ImGui::BeginPopupContextItem("Editor")) {
            ImGui::SliderFloat("p0.y", &curve.p0.y, 0.0f, 1.0f);
            ImGui::SliderFloat2("p1", glm::value_ptr(curve.p1), 0.0f, 1.0f);
            ImGui::SliderFloat2("p2", glm::value_ptr(curve.p2), 0.0f, 1.0f);
            ImGui::SliderFloat("p3.y", &curve.p3.y, 0.0f, 1.0f);
            curveEditor("CurveEditor", curve, points);
            ImGui::EndPopup();
        }

        ImGui::InvisibleButton(label, frameSize);
        ImGui::OpenPopupOnItemClick("Editor", ImGuiPopupFlags_MouseButtonLeft);

        ImGui::PopID();
    }
}

#endif //IMP_IMGUIPLOTS_H
