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
        _text = std::format("{:.0f}", lightLevel <= 0.0f ? 0.0f : std::ceil(lightLevel / 10.0f) * 10.0f);
    }

    std::string LightIconWidget::widgetConfigName() { return "LightIconWidget"; }

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
    }
}