#include "StaminaRegenWidget.h"

using namespace ImGuiMCP;

namespace InfoWidgets
{

    std::string StaminaRegenTextWidget::widgetConfigName() { return "StaminaRegenTextWidget"; }

    void StaminaRegenTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
        {
            _text = "";
            return;
        }
        const float maxStamina = player->AsActorValueOwner()->GetPermanentActorValue(RE::ActorValue::kStamina);
        const float regenRate = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kStaminaRate);
        const float regenMult = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kStaminaRateMult);
        const float regenPerSecond = maxStamina * (regenRate / 100.0f) * (regenMult / 100.0f);
        _text = std::format("{:.1f}/s", regenPerSecond);
    }

    std::string StaminaRegenIconWidget::widgetConfigName() { return "StaminaRegenIconWidget"; }

    void StaminaRegenIconWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
        {
            _text = "";
            return;
        }
        _text = ICON_FA_LUNGS;
    }
}
