#include <cmath>
#include <format>

#include "IconsFontAwesome6.h"

#include "ShoutCooldownWidget.h"

namespace InfoWidgets
{
    float ShoutCooldownMixin::remainingShoutCooldown() const
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        float remaining = player ? player->GetVoiceRecoveryTime() : 0.0f;
        return remaining < 0.0f ? 0.0f : remaining;
    }

    void ShoutCooldownMixin::applyCooldownColor(std::optional<ImGuiMCP::ImVec4> &valueColor, float remaining)
    {
        if (remaining > 10.0f)
            valueColor = _highCooldownColor;
        else if (remaining > 5.0f)
            valueColor = _mediumCooldownColor;
        else if (remaining > 0.0f)
            valueColor = _lowCooldownColor;
        else
            valueColor = std::nullopt;
    }

    void ShoutCooldownMixin::configureCooldownColors(const toml::table &root, const std::string &section)
    {
        _highCooldownColor = ColorConfig::loadColorFromConfig(root.at_path(section + ".highCooldownColor"), _highCooldownColor);
        _mediumCooldownColor = ColorConfig::loadColorFromConfig(root.at_path(section + ".mediumCooldownColor"), _mediumCooldownColor);
        _lowCooldownColor = ColorConfig::loadColorFromConfig(root.at_path(section + ".lowCooldownColor"), _lowCooldownColor);
    }

    void ShoutCooldownMixin::saveCooldownColors(toml::table &root, const std::string &section)
    {
        auto &sec = *root.get_as<toml::table>(section);
        if (!sec.contains("highCooldownColor"))
            sec.insert("highCooldownColor", toml::table{});
        if (!sec.contains("mediumCooldownColor"))
            sec.insert("mediumCooldownColor", toml::table{});
        if (!sec.contains("lowCooldownColor"))
            sec.insert("lowCooldownColor", toml::table{});
        ColorConfig::saveColorToConfig(_highCooldownColor, *sec.get_as<toml::table>("highCooldownColor"));
        ColorConfig::saveColorToConfig(_mediumCooldownColor, *sec.get_as<toml::table>("mediumCooldownColor"));
        ColorConfig::saveColorToConfig(_lowCooldownColor, *sec.get_as<toml::table>("lowCooldownColor"));
    }

    bool ShoutCooldownMixin::renderCooldownColors()
    {
        bool changed = ImGuiMCP::ImGui::ColorEdit4("High Cooldown Color (> 10s)", &_highCooldownColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        changed |= ImGuiMCP::ImGui::ColorEdit4("Medium Cooldown Color (5-10s)", &_mediumCooldownColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        changed |= ImGuiMCP::ImGui::ColorEdit4("Low Cooldown Color (< 5s)", &_lowCooldownColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        return changed;
    }

    void ShoutCooldownMixin::configureHideWhenReady(const toml::table &root, const std::string &section)
    {
        _hideWhenReady = root.at_path(section + ".hideWhenReady").value_or(_hideWhenReady);
    }

    void ShoutCooldownMixin::saveHideWhenReady(toml::table &root, const std::string &section)
    {
        root.get_as<toml::table>(section)->insert_or_assign("hideWhenReady", _hideWhenReady);
    }

    bool ShoutCooldownMixin::renderHideWhenReady()
    {
        return ImGuiMCP::ImGui::Checkbox("Hide When Ready", &_hideWhenReady);
    }

    bool ShoutCooldownMixin::isShoutSelected() const
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
            return false;
        auto *selectedPower = player->GetActorRuntimeData().selectedPower;
        return selectedPower && selectedPower->As<RE::TESShout>() != nullptr;
    }

    void ShoutCooldownMixin::configureRequireShoutSelected(const toml::table &root, const std::string &section)
    {
        _requireShoutSelected = root.at_path(section + ".requireShoutSelected").value_or(_requireShoutSelected);
    }

    void ShoutCooldownMixin::saveRequireShoutSelected(toml::table &root, const std::string &section)
    {
        root.get_as<toml::table>(section)->insert_or_assign("requireShoutSelected", _requireShoutSelected);
    }

    bool ShoutCooldownMixin::renderRequireShoutSelected()
    {
        return ImGuiMCP::ImGui::Checkbox("Only Show When Shout Selected", &_requireShoutSelected);
    }

    // ---- ShoutCooldownTextWidget ----

    std::string ShoutCooldownTextWidget::widgetConfigName() { return "ShoutCooldownTextWidget"; }

    void ShoutCooldownTextWidget::configure(const toml::table &root)
    {
        TextWidget::configure(root);
        configureCooldownColors(root, widgetConfigName());
        configureHideWhenReady(root, widgetConfigName());
        configureRequireShoutSelected(root, widgetConfigName());
    }

    void ShoutCooldownTextWidget::saveConfig(toml::table &root)
    {
        TextWidget::saveConfig(root);
        saveCooldownColors(root, widgetConfigName());
        saveHideWhenReady(root, widgetConfigName());
        saveRequireShoutSelected(root, widgetConfigName());
    }

    bool ShoutCooldownTextWidget::renderConfig(toml::table &root)
    {
        bool changed = TextWidget::renderConfig(root);
        changed |= renderCooldownColors();
        changed |= renderHideWhenReady();
        changed |= renderRequireShoutSelected();
        return changed;
    }

    void ShoutCooldownTextWidget::update()
    {
        const float remaining = remainingShoutCooldown();

        if ((remaining <= 0.0f && hideWhenReady()) || (requireShoutSelected() && !isShoutSelected()))
        {
            _text = "";
            return;
        }

        _text = std::format("{:.0f}", std::ceil(remaining));
        applyCooldownColor(_valueColor, remaining);
    }

    // ---- ShoutCooldownIconWidget ----

    ShoutCooldownIconWidget::ShoutCooldownIconWidget()
    {
        _icon = _defaultIcon = ICON_FA_DRAGON;
    }

    std::string ShoutCooldownIconWidget::widgetConfigName() { return "ShoutCooldownIconWidget"; }

    void ShoutCooldownIconWidget::configure(const toml::table &root)
    {
        IconWidget::configure(root);
        configureCooldownColors(root, widgetConfigName());
        configureHideWhenReady(root, widgetConfigName());
        configureRequireShoutSelected(root, widgetConfigName());
    }

    void ShoutCooldownIconWidget::saveConfig(toml::table &root)
    {
        IconWidget::saveConfig(root);
        saveCooldownColors(root, widgetConfigName());
        saveHideWhenReady(root, widgetConfigName());
        saveRequireShoutSelected(root, widgetConfigName());
    }

    bool ShoutCooldownIconWidget::renderConfig(toml::table &root)
    {
        bool changed = IconWidget::renderConfig(root);
        changed |= renderCooldownColors();
        changed |= renderHideWhenReady();
        changed |= renderRequireShoutSelected();
        return changed;
    }

    void ShoutCooldownIconWidget::update()
    {
        const float remaining = remainingShoutCooldown();

        if ((remaining <= 0.0f && hideWhenReady()) || (requireShoutSelected() && !isShoutSelected()))
        {
            _text = "";
            return;
        }

        _text = _icon;
        applyCooldownColor(_valueColor, remaining);
    }
}
