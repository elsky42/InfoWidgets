#include "StaminaRegenWidget.h"

#include "FlatRegenCache.h"

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
        const float regenPerSecond = maxStamina * (regenRate / 100.0f) * (regenMult / 100.0f)
                                   + FlatRegenCache::get().stamina;
        _text = std::format("{:.1f}/s", regenPerSecond);
    }

    StaminaRegenIconWidget::StaminaRegenIconWidget()
    {
        _icon = _defaultIcon = ICON_FA_LUNGS;
    }

    std::string StaminaRegenIconWidget::widgetConfigName() { return "StaminaRegenIconWidget"; }
}
