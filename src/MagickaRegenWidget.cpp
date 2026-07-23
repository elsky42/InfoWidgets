#include "MagickaRegenWidget.h"

#include "FlatRegenCache.h"

using namespace ImGuiMCP;

namespace InfoWidgets
{

    std::string MagickaRegenTextWidget::widgetConfigName() { return "MagickaRegenTextWidget"; }

    void MagickaRegenTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
        {
            _text = "";
            return;
        }
        const float maxMagicka = player->AsActorValueOwner()->GetPermanentActorValue(RE::ActorValue::kMagicka);
        const float regenRate = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMagickaRate);
        const float regenMult = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMagickaRateMult);
        const float regenPerSecond = maxMagicka * (regenRate / 100.0f) * (regenMult / 100.0f)
                                   + FlatRegenCache::get().magicka;
        _text = std::format("{:.1f}/s", regenPerSecond);
    }

    MagickaRegenIconWidget::MagickaRegenIconWidget()
    {
        _icon = _defaultIcon = ICON_FA_WATER;
    }

    std::string MagickaRegenIconWidget::widgetConfigName() { return "MagickaRegenIconWidget"; }
}
