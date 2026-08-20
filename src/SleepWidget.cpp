#include "IconsFontAwesome6.h"

#include "SleepWidget.h"

namespace InfoWidgets
{
    SleepIconWidget::SleepIconWidget()
    {
        _icon = _defaultIcon = ICON_FA_BED;
    }

    std::string SleepIconWidget::widgetConfigName() { return "SleepIconWidget"; }

    void SleepIconWidget::configure(const toml::table &root)
    {
        IconWidget::configure(root);
        auto s = widgetConfigName();
        _notRestedColor = ColorConfig::loadColorFromConfig(root.at_path(s + ".notRestedColor"), _notRestedColor);
        _restedColor = ColorConfig::loadColorFromConfig(root.at_path(s + ".restedColor"), _restedColor);
        _wellRestedColor = ColorConfig::loadColorFromConfig(root.at_path(s + ".wellRestedColor"), _wellRestedColor);
        _loversComfortColor = ColorConfig::loadColorFromConfig(root.at_path(s + ".loversComfortColor"), _loversComfortColor);
        _werewolfColor = ColorConfig::loadColorFromConfig(root.at_path(s + ".werewolfColor"), _werewolfColor);
        _hideWhenWellRested = root.at_path(s + ".hideWhenWellRested").value_or(_hideWhenWellRested);
        _hideWhenWerewolf = root.at_path(s + ".hideWhenWerewolf").value_or(_hideWhenWerewolf);
    }

    void SleepIconWidget::saveConfig(toml::table &root)
    {
        IconWidget::saveConfig(root);
        auto &sec = *root.get_as<toml::table>(widgetConfigName());
        if (!sec.contains("notRestedColor"))
            sec.insert("notRestedColor", toml::table{});
        if (!sec.contains("restedColor"))
            sec.insert("restedColor", toml::table{});
        if (!sec.contains("wellRestedColor"))
            sec.insert("wellRestedColor", toml::table{});
        if (!sec.contains("loversComfortColor"))
            sec.insert("loversComfortColor", toml::table{});
        if (!sec.contains("werewolfColor"))
            sec.insert("werewolfColor", toml::table{});
        ColorConfig::saveColorToConfig(_notRestedColor, *sec.get_as<toml::table>("notRestedColor"));
        ColorConfig::saveColorToConfig(_restedColor, *sec.get_as<toml::table>("restedColor"));
        ColorConfig::saveColorToConfig(_wellRestedColor, *sec.get_as<toml::table>("wellRestedColor"));
        ColorConfig::saveColorToConfig(_loversComfortColor, *sec.get_as<toml::table>("loversComfortColor"));
        ColorConfig::saveColorToConfig(_werewolfColor, *sec.get_as<toml::table>("werewolfColor"));
        sec.insert_or_assign("hideWhenWellRested", _hideWhenWellRested);
        sec.insert_or_assign("hideWhenWerewolf", _hideWhenWerewolf);
    }

    bool SleepIconWidget::renderConfig(toml::table &root)
    {
        bool changed = IconWidget::renderConfig(root);
        changed |= ImGuiMCP::ImGui::ColorEdit4("Not Rested Color", &_notRestedColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        changed |= ImGuiMCP::ImGui::ColorEdit4("Rested Color", &_restedColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        changed |= ImGuiMCP::ImGui::ColorEdit4("Well Rested Color", &_wellRestedColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        changed |= ImGuiMCP::ImGui::ColorEdit4("Lover's Comfort Color", &_loversComfortColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        changed |= ImGuiMCP::ImGui::ColorEdit4("Werewolf Color", &_werewolfColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        changed |= ImGuiMCP::ImGui::Checkbox("Hide When Well Rested", &_hideWhenWellRested);
        changed |= ImGuiMCP::ImGui::Checkbox("Hide When Werewolf", &_hideWhenWerewolf);
        return changed;
    }

    SleepLevel SleepIconWidget::sleepLevel()
    {
        if (_unavailable)
            return SleepLevel::NotRested;

        if (!_restedSpell || !_wellRestedSpell || !_loversComfortSpell || !_werewolfSpell)
        {
            auto *handler = RE::TESDataHandler::GetSingleton();
            _restedSpell = handler->LookupForm<RE::SpellItem>(0x0FB981, "Skyrim.esm");
            _wellRestedSpell = handler->LookupForm<RE::SpellItem>(0x0FB984, "Skyrim.esm");
            _loversComfortSpell = handler->LookupForm<RE::SpellItem>(0x0CDA1D, "Skyrim.esm");
            _werewolfSpell = handler->LookupForm<RE::SpellItem>(0x0A1A3E, "Skyrim.esm");
            if (!_restedSpell || !_wellRestedSpell || !_loversComfortSpell || !_werewolfSpell)
            {
                SKSE::log::error("SleepIconWidget: failed to look up sleep forms from Skyrim.esm");
                _unavailable = true;
                return SleepLevel::NotRested;
            }
        }

        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
            return SleepLevel::NotRested;

        if (player->HasSpell(_werewolfSpell))
            return SleepLevel::Werewolf;

        if (player->HasSpell(_loversComfortSpell))
            return SleepLevel::LoversComfort;
        if (player->HasSpell(_wellRestedSpell))
            return SleepLevel::WellRested;
        if (player->HasSpell(_restedSpell))
            return SleepLevel::Rested;
        return SleepLevel::NotRested;
    }

    void SleepIconWidget::update()
    {
        auto level = sleepLevel();

        if (level == SleepLevel::Werewolf)
        {
            _text = _hideWhenWerewolf ? "" : ICON_FA_PAW;
            _valueColor = _werewolfColor;
            return;
        }

        if (_hideWhenWellRested && (level == SleepLevel::WellRested || level == SleepLevel::LoversComfort))
        {
            _text = "";
            return;
        }

        _text = _icon;
        switch (level)
        {
        case SleepLevel::Rested:
            _valueColor = _restedColor;
            break;
        case SleepLevel::WellRested:
            _valueColor = _wellRestedColor;
            break;
        case SleepLevel::LoversComfort:
            _valueColor = _loversComfortColor;
            break;
        default:
            _valueColor = _notRestedColor;
            break;
        }
    }
}
