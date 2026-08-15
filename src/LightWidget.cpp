#include "LightWidget.h"

using namespace ImGuiMCP;

namespace InfoWidgets
{
    float GetPlayerLightLevel()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
            return 0.0f;

        // process->high->lightLevel does not match what GetLightLevel() (console/Papyrus)
        // reports, by any consistent formula, so call the same underlying function instead.
        // "player.getlightlevel" resolves through the script-function table (the one legacy
        // scripts/conditions call functions through), not the separate console-command list.
        static RE::SCRIPT_FUNCTION *getLightLevelFunc = nullptr;
        if (!getLightLevelFunc)
            getLightLevelFunc = RE::SCRIPT_FUNCTION::LocateScriptCommand("GetLightLevel");
        if (!getLightLevelFunc || !getLightLevelFunc->conditionFunction)
        {
            SKSE::log::error("LightWidget: could not locate the GetLightLevel script function, widget will show 0");
            return 0.0f;
        }

        double result = 0.0;
        if (!getLightLevelFunc->conditionFunction(player, nullptr, nullptr, result))
            return 0.0f;

        return static_cast<float>(result);
    }

    std::string LightTextWidget::widgetConfigName() { return "LightTextWidget"; }

    void LightTextWidget::configure(const toml::table &root)
    {
        TextWidget::configure(root);
        configureLevelColors(root, widgetConfigName());
    }

    void LightTextWidget::saveConfig(toml::table &root)
    {
        TextWidget::saveConfig(root);
        saveLevelColors(root, widgetConfigName());
    }

    bool LightTextWidget::renderConfig(toml::table &root)
    {
        bool changed = TextWidget::renderConfig(root);
        changed |= renderLevelColors();
        return changed;
    }

    void LightTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player || !player->IsSneaking())
        {
            _text = "";
            return;
        }
        const float lightLevel = GetPlayerLightLevel();
        const float roundedLightLevel = lightLevel <= 0.0f ? 0.0f : std::ceil(lightLevel);
        _text = std::format("{:.0f}", roundedLightLevel);
        applyLevelColor(_valueColor, roundedLightLevel);
    }

    std::string LightIconWidget::widgetConfigName() { return "LightIconWidget"; }

    void LightIconWidget::configure(const toml::table &root)
    {
        IconWidget::configure(root);
        configureLevelColors(root, widgetConfigName());
    }

    void LightIconWidget::saveConfig(toml::table &root)
    {
        IconWidget::saveConfig(root);
        saveLevelColors(root, widgetConfigName());
    }

    bool LightIconWidget::renderConfig(toml::table &root)
    {
        bool changed = IconWidget::renderConfig(root);
        changed |= renderLevelColors();
        return changed;
    }

    void LightIconWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player || !player->IsSneaking())
        {
            _text = "";
            return;
        }
        const float lightLevel = GetPlayerLightLevel();
        if (lightLevel <= 0)
        {
            _text = ICON_FA_BATTERY_EMPTY;
        }
        else if (lightLevel <= 25)
        {
            _text = ICON_FA_BATTERY_QUARTER;
        }
        else if (lightLevel <= 50)
        {
            _text = ICON_FA_BATTERY_HALF;
        }
        else if (lightLevel <= 75)
        {
            _text = ICON_FA_BATTERY_THREE_QUARTERS;
        }
        else
        {
            _text = ICON_FA_BATTERY_FULL;
        }
        applyLevelColor(_valueColor, lightLevel);
    }
}