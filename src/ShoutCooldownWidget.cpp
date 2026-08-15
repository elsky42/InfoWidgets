#include <cmath>
#include <format>

#include "ShoutCooldownWidget.h"

namespace InfoWidgets
{
    std::string ShoutCooldownTextWidget::widgetConfigName() { return "ShoutCooldownTextWidget"; }

    void ShoutCooldownTextWidget::configure(const toml::table &root)
    {
        TextWidget::configure(root);
        auto s = widgetConfigName();
        _highCooldownColor = ColorConfig::loadColorFromConfig(root.at_path(s + ".highCooldownColor"), _highCooldownColor);
        _mediumCooldownColor = ColorConfig::loadColorFromConfig(root.at_path(s + ".mediumCooldownColor"), _mediumCooldownColor);
        _lowCooldownColor = ColorConfig::loadColorFromConfig(root.at_path(s + ".lowCooldownColor"), _lowCooldownColor);
        _hideWhenReady = root.at_path(s + ".hideWhenReady").value_or(_hideWhenReady);
    }

    void ShoutCooldownTextWidget::saveConfig(toml::table &root)
    {
        TextWidget::saveConfig(root);
        auto &sec = *root.get_as<toml::table>(widgetConfigName());
        if (!sec.contains("highCooldownColor"))
            sec.insert("highCooldownColor", toml::table{});
        if (!sec.contains("mediumCooldownColor"))
            sec.insert("mediumCooldownColor", toml::table{});
        if (!sec.contains("lowCooldownColor"))
            sec.insert("lowCooldownColor", toml::table{});
        ColorConfig::saveColorToConfig(_highCooldownColor, *sec.get_as<toml::table>("highCooldownColor"));
        ColorConfig::saveColorToConfig(_mediumCooldownColor, *sec.get_as<toml::table>("mediumCooldownColor"));
        ColorConfig::saveColorToConfig(_lowCooldownColor, *sec.get_as<toml::table>("lowCooldownColor"));
        sec.insert_or_assign("hideWhenReady", _hideWhenReady);
    }

    bool ShoutCooldownTextWidget::renderConfig(toml::table &root)
    {
        bool changed = TextWidget::renderConfig(root);
        changed |= ImGuiMCP::ImGui::ColorEdit4("High Cooldown Color (> 10s)", &_highCooldownColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        changed |= ImGuiMCP::ImGui::ColorEdit4("Medium Cooldown Color (5-10s)", &_mediumCooldownColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        changed |= ImGuiMCP::ImGui::ColorEdit4("Low Cooldown Color (< 5s)", &_lowCooldownColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        changed |= ImGuiMCP::ImGui::Checkbox("Hide When Ready", &_hideWhenReady);
        return changed;
    }

    void ShoutCooldownTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        float remaining = player ? player->GetVoiceRecoveryTime() : 0.0f;
        if (remaining < 0.0f)
            remaining = 0.0f;

        if (remaining <= 0.0f && _hideWhenReady)
        {
            _text = "";
            return;
        }

        _text = std::format("{:.0f}", std::ceil(remaining));

        if (remaining > 10.0f)
            _valueColor = _highCooldownColor;
        else if (remaining > 5.0f)
            _valueColor = _mediumCooldownColor;
        else if (remaining > 0.0f)
            _valueColor = _lowCooldownColor;
        else
            _valueColor = std::nullopt;
    }
}
