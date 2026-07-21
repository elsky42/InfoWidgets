#include "MovementSpeedWidget.h"

using namespace ImGuiMCP;

namespace InfoWidgets
{

    std::string MovementSpeedTextWidget::widgetConfigName() { return "MovementSpeedTextWidget"; }

    void MovementSpeedTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) { _text = ""; return; }
        const float speed = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kSpeedMult);
        _text = std::format("{:.0f}", speed);
    }

    std::string MovementSpeedIconWidget::widgetConfigName() { return "MovementSpeedIconWidget"; }

    void MovementSpeedIconWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) { _text = ""; return; }
        _text = ICON_FA_SHOE_PRINTS;
    }
}
