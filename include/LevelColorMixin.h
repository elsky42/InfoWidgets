#pragma once

#include <optional>

#include "ColorConfig.h"

namespace InfoWidgets
{
    class LevelColorMixin
    {
    protected:
        void applyLevelColor(std::optional<ImGuiMCP::ImVec4> &valueColor, float value);
        void configureLevelColors(const toml::table &root, const std::string &section);
        void saveLevelColors(toml::table &root, const std::string &section);
        bool renderLevelColors();

        ImGuiMCP::ImVec4 _lowColor{1.0f, 1.0f, 0.0f, 1.0f};
        ImGuiMCP::ImVec4 _mediumColor{1.0f, 0.55f, 0.0f, 1.0f};
        ImGuiMCP::ImVec4 _highColor{0.9f, 0.1f, 0.1f, 1.0f};
    };
}
