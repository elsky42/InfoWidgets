#include <format>

#include "IconsFontAwesome6.h"

#include "ToxicityWidget.h"

namespace InfoWidgets
{
    bool ToxicityColorMixin::fetchToxicityData(float &used, float &max, const std::string &widgetName)
    {
        if (_unavailable)
            return false;

        if (!_toxicitySlotsAvailable)
        {
            auto *form = RE::TESForm::LookupByEditorID("ToxicitySlots_Forn");
            if (form)
                _toxicitySlotsAvailable = form->As<RE::TESGlobal>();

            if (!_toxicitySlotsAvailable)
            {
                SKSE::log::warn("{}: global variable ToxicitySlots_Forn not found, widget will be hidden", widgetName);
                _unavailable = true;
                return false;
            }
        }

        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
            return false;

        used = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kVariable05);
        max = _toxicitySlotsAvailable->value;
        return true;
    }

    void ToxicityColorMixin::applyToxicityColor(
        std::optional<ImGuiMCP::ImVec4> &valueColor, float used, float max)
    {
        if (used > max)
            valueColor = _overdoseColor;
        else if (used >= max)
            valueColor = _dangerColor;
        else if (used >= max - 1.0f)
            valueColor = _warningColor;
        else
            valueColor = std::nullopt;
    }

    void ToxicityColorMixin::configureToxicityColors(const toml::table &root, const std::string &section)
    {
        _warningColor = ColorConfig::loadColorFromConfig(root.at_path(section + ".warningColor"), _warningColor);
        _dangerColor = ColorConfig::loadColorFromConfig(root.at_path(section + ".dangerColor"), _dangerColor);
        _overdoseColor = ColorConfig::loadColorFromConfig(root.at_path(section + ".overdoseColor"), _overdoseColor);
    }

    void ToxicityColorMixin::saveToxicityColors(toml::table &root, const std::string &section)
    {
        auto &sec = *root.get_as<toml::table>(section);
        if (!sec.contains("warningColor"))
            sec.insert("warningColor", toml::table{});
        if (!sec.contains("dangerColor"))
            sec.insert("dangerColor", toml::table{});
        if (!sec.contains("overdoseColor"))
            sec.insert("overdoseColor", toml::table{});
        ColorConfig::saveColorToConfig(_warningColor, *sec.get_as<toml::table>("warningColor"));
        ColorConfig::saveColorToConfig(_dangerColor, *sec.get_as<toml::table>("dangerColor"));
        ColorConfig::saveColorToConfig(_overdoseColor, *sec.get_as<toml::table>("overdoseColor"));
    }

    bool ToxicityColorMixin::renderToxicityColors()
    {
        bool changed = ImGuiMCP::ImGui::ColorEdit4("Warning Color", &_warningColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        changed |= ImGuiMCP::ImGui::ColorEdit4("Danger Color", &_dangerColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        changed |= ImGuiMCP::ImGui::ColorEdit4("Overdose Color", &_overdoseColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        return changed;
    }

    std::string ToxicityTextWidget::widgetConfigName() { return "ToxicityTextWidget"; }

    void ToxicityTextWidget::configure(const toml::table &root)
    {
        TextWidget::configure(root);
        configureToxicityColors(root, widgetConfigName());
    }

    void ToxicityTextWidget::saveConfig(toml::table &root)
    {
        TextWidget::saveConfig(root);
        saveToxicityColors(root, widgetConfigName());
    }

    bool ToxicityTextWidget::renderConfig(toml::table &root)
    {
        bool changed = TextWidget::renderConfig(root);
        changed |= renderToxicityColors();
        return changed;
    }

    void ToxicityTextWidget::update()
    {
        float used, max;
        if (!fetchToxicityData(used, max, widgetConfigName()))
        {
            _text = "";
            return;
        }
        _text = std::format("{} / {}", used, max);
        applyToxicityColor(_valueColor, used, max);
    }

    // ---- ToxicityIconWidget ----

    ToxicityIconWidget::ToxicityIconWidget()
    {
        _text = ICON_FA_BOTTLE_DROPLET;
    }

    std::string ToxicityIconWidget::widgetConfigName() { return "ToxicityIconWidget"; }

    void ToxicityIconWidget::configure(const toml::table &root)
    {
        IconWidget::configure(root);
        configureToxicityColors(root, widgetConfigName());
    }

    void ToxicityIconWidget::saveConfig(toml::table &root)
    {
        IconWidget::saveConfig(root);
        saveToxicityColors(root, widgetConfigName());
    }

    bool ToxicityIconWidget::renderConfig(toml::table &root)
    {
        bool changed = IconWidget::renderConfig(root);
        changed |= renderToxicityColors();
        return changed;
    }

    void ToxicityIconWidget::update()
    {
        float used, max;
        if (!fetchToxicityData(used, max, widgetConfigName()))
            return;
        applyToxicityColor(_valueColor, used, max);
    }
}
