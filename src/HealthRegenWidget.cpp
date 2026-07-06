#include "HealthRegenWidget.h"

using namespace ImGuiMCP;

namespace InfoWidgets
{

    std::string HealthRegenTextWidget::widgetConfigName() { return "HealthRegenTextWidget"; }

    void HealthRegenTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
        {
            _text = "";
            return;
        }
        const float maxHealth = player->AsActorValueOwner()->GetPermanentActorValue(RE::ActorValue::kHealth);
        const float regenRate = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHealRate);
        const float regenMult = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHealRateMult);
        const float regenPerSecond = maxHealth * (regenRate / 100.0f) * (regenMult / 100.0f);
        _text = std::format("{:.1f}/s", regenPerSecond);
    }

    std::string HealthRegenIconWidget::widgetConfigName() { return "HealthRegenIconWidget"; }

    void HealthRegenIconWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
        {
            _text = "";
            return;
        }
        _text = ICON_FA_HEART;
    }
}
