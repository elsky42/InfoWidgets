#include <format>

#include "IconsFontAwesome6.h"

#include "CarryWeightWidget.h"

namespace InfoWidgets
{
    void CarryWeightColorMixin::applyCarryWeightColor(
        std::optional<ImGuiMCP::ImVec4> &valueColor, float current, float max)
    {
        if (current >= max)
            valueColor = _criticalColor;
        else if (current >= max - 5.0f)
            valueColor = _warningColor;
        else
            valueColor = std::nullopt;
    }

    void CarryWeightColorMixin::configureCarryWeightColors(const toml::table &root, const std::string &section)
    {
        _warningColor = ColorConfig::loadColorFromConfig(root.at_path(section + ".warningColor"), _warningColor);
        _criticalColor = ColorConfig::loadColorFromConfig(root.at_path(section + ".criticalColor"), _criticalColor);
    }

    void CarryWeightColorMixin::saveCarryWeightColors(toml::table &root, const std::string &section)
    {
        auto &sec = *root.get_as<toml::table>(section);
        if (!sec.contains("warningColor"))
            sec.insert("warningColor", toml::table{});
        if (!sec.contains("criticalColor"))
            sec.insert("criticalColor", toml::table{});
        ColorConfig::saveColorToConfig(_warningColor, *sec.get_as<toml::table>("warningColor"));
        ColorConfig::saveColorToConfig(_criticalColor, *sec.get_as<toml::table>("criticalColor"));
    }

    bool CarryWeightColorMixin::renderCarryWeightColors()
    {
        bool changed = ImGuiMCP::ImGui::ColorEdit4("Warning Color", &_warningColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        changed |= ImGuiMCP::ImGui::ColorEdit4("Critical Color", &_criticalColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        return changed;
    }

    std::string CarryWeightTextWidget::widgetConfigName() { return "CarryWeightTextWidget"; }

    void CarryWeightTextWidget::configure(const toml::table &root)
    {
        TextWidget::configure(root);
        configureCarryWeightColors(root, widgetConfigName());
    }

    void CarryWeightTextWidget::saveConfig(toml::table &root)
    {
        TextWidget::saveConfig(root);
        saveCarryWeightColors(root, widgetConfigName());
    }

    bool CarryWeightTextWidget::renderConfig(toml::table &root)
    {
        bool changed = TextWidget::renderConfig(root);
        changed |= renderCarryWeightColors();
        return changed;
    }

    void CarryWeightTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
        {
            _text = "";
            return;
        }
        auto *avOwner = player->AsActorValueOwner();
        const float current = avOwner->GetActorValue(RE::ActorValue::kInventoryWeight);
        const float max = avOwner->GetActorValue(RE::ActorValue::kCarryWeight);
        _text = std::format("{:.0f} / {:.0f}", current, max);
        applyCarryWeightColor(_valueColor, current, max);
    }

    CarryWeightIconWidget::CarryWeightIconWidget()
    {
        _icon = _defaultIcon = ICON_FA_WEIGHT_HANGING;
    }

    std::string CarryWeightIconWidget::widgetConfigName() { return "CarryWeightIconWidget"; }

    void CarryWeightIconWidget::configure(const toml::table &root)
    {
        IconWidget::configure(root);
        configureCarryWeightColors(root, widgetConfigName());
    }

    void CarryWeightIconWidget::saveConfig(toml::table &root)
    {
        IconWidget::saveConfig(root);
        saveCarryWeightColors(root, widgetConfigName());
    }

    bool CarryWeightIconWidget::renderConfig(toml::table &root)
    {
        bool changed = IconWidget::renderConfig(root);
        changed |= renderCarryWeightColors();
        return changed;
    }

    void CarryWeightIconWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
            return;
        auto *avOwner = player->AsActorValueOwner();
        const float current = avOwner->GetActorValue(RE::ActorValue::kInventoryWeight);
        const float max = avOwner->GetActorValue(RE::ActorValue::kCarryWeight);
        applyCarryWeightColor(_valueColor, current, max);
    }
}
