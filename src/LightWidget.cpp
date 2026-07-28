#include "LightWidget.h"

using namespace ImGuiMCP;

namespace InfoWidgets
{
    float GetPlayerLightLevel()
    {
        const auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
            return 0.0f;

        const auto *process = player->GetActorRuntimeData().currentProcess;
        if (!process || !process->high)
            return 0.0f;

        return process->high->lightLevel;
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
        // too many updates so erase the first digit
        const float roundedLightLevel = lightLevel <= 0.0f ? 0.0f : std::ceil(lightLevel / 10.0f) * 10.0f;
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