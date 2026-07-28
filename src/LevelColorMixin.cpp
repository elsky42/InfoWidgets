#include "LevelColorMixin.h"

namespace InfoWidgets
{
    void LevelColorMixin::applyLevelColor(std::optional<ImGuiMCP::ImVec4> &valueColor, float value)
    {
        if (value <= 0.0f)
            valueColor = std::nullopt;
        else if (value <= 25.0f)
            valueColor = _lowColor;
        else if (value <= 50.0f)
            valueColor = _mediumColor;
        else
            valueColor = _highColor;
    }

    void LevelColorMixin::configureLevelColors(const toml::table &root, const std::string &section)
    {
        _lowColor = ColorConfig::loadColorFromConfig(root.at_path(section + ".lowColor"), _lowColor);
        _mediumColor = ColorConfig::loadColorFromConfig(root.at_path(section + ".mediumColor"), _mediumColor);
        _highColor = ColorConfig::loadColorFromConfig(root.at_path(section + ".highColor"), _highColor);
    }

    void LevelColorMixin::saveLevelColors(toml::table &root, const std::string &section)
    {
        auto &sec = *root.get_as<toml::table>(section);
        if (!sec.contains("lowColor"))
            sec.insert("lowColor", toml::table{});
        if (!sec.contains("mediumColor"))
            sec.insert("mediumColor", toml::table{});
        if (!sec.contains("highColor"))
            sec.insert("highColor", toml::table{});
        ColorConfig::saveColorToConfig(_lowColor, *sec.get_as<toml::table>("lowColor"));
        ColorConfig::saveColorToConfig(_mediumColor, *sec.get_as<toml::table>("mediumColor"));
        ColorConfig::saveColorToConfig(_highColor, *sec.get_as<toml::table>("highColor"));
    }

    bool LevelColorMixin::renderLevelColors()
    {
        bool changed = ImGuiMCP::ImGui::ColorEdit4("Low Color", &_lowColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        changed |= ImGuiMCP::ImGui::ColorEdit4("Medium Color", &_mediumColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        changed |= ImGuiMCP::ImGui::ColorEdit4("High Color", &_highColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        return changed;
    }
}
