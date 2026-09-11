#include <format>

#include "IconsFontAwesome6.h"

#include "MassWidget.h"

namespace InfoWidgets
{
    std::string MassTextWidget::widgetConfigName() { return "MassTextWidget"; }

    void MassTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
        {
            _text = "";
            return;
        }
        const float mass = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMass);
        _text = std::format("{:.2f}", mass);
    }

    MassIconWidget::MassIconWidget()
    {
        _icon = _defaultIcon = ICON_FA_WEIGHT_SCALE;
    }

    std::string MassIconWidget::widgetConfigName() { return "MassIconWidget"; }
}
