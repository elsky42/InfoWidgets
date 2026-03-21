#include "NoiseWidget.h"

using namespace ImGuiMCP;

namespace InfoWidgets
{

    std::string NoiseTextWidget::widgetConfigName() { return "NoiseTextWidget"; }

    void NoiseTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player || !player->IsSneaking())
        {
            _text = "";
            return;
        }
        const float noise = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMovementNoiseMult);
        _text = std::format("{:.0f}", std::ceil(noise * 100.0f));
    }

    std::string NoiseIconWidget::widgetConfigName() { return "NoiseIconWidget"; }

    void NoiseIconWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player || !player->IsSneaking())
        {
            _text = "";
            return;
        }
        const float noise = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMovementNoiseMult);
        if (noise < 0)
        {
            _text = ICON_FA_VOLUME_OFF;
        }
        else if (noise < 0.25)
        {
            _text = ICON_FA_VOLUME_LOW;
        }
        else
        {
            _text = ICON_FA_VOLUME_HIGH;
        }
    }
}