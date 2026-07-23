#include "SpellPowerWidget.h"

namespace InfoWidgets
{

    std::string SpellPowerTextWidget::widgetConfigName() { return "SpellPowerTextWidget"; }

    void SpellPowerTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
        {
            _text = "";
            return;
        }
        // 3BFTweaks accumulates spell power (Dunmer heritage, Azura's teachings, the
        // Ritual Stone, etc.) into the otherwise-unused kDestructionSkillAdvance actor
        // value rather than exposing a dedicated one.
        const float spellPower = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kDestructionSkillAdvance);
        _text = std::format("{:.0f}%", spellPower);
    }

    SpellPowerIconWidget::SpellPowerIconWidget()
    {
        _icon = _defaultIcon = ICON_FA_WAND_MAGIC_SPARKLES;
    }

    std::string SpellPowerIconWidget::widgetConfigName() { return "SpellPowerIconWidget"; }
}
