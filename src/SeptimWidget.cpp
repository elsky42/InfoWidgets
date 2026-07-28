#include "SeptimWidget.h"

#undef GetObject

using namespace ImGuiMCP;

namespace InfoWidgets
{

    std::string SeptimTextWidget::widgetConfigName() { return "SeptimTextWidget"; }

    void SeptimTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) { _text = ""; return; }

        auto *gold = RE::TESForm::LookupByID<RE::TESBoundObject>(0x0000000F);
        if (!gold) { _text = ""; return; }

        auto *invChanges = player->GetInventoryChanges(true);
        std::int32_t amount = 0;
        if (invChanges && invChanges->entryList)
        {
            for (auto *entry : *invChanges->entryList)
            {
                if (entry && entry->GetObject() == gold)
                    amount += entry->countDelta;
            }
        }
        _text = std::format("{}", amount);
    }

    SeptimIconWidget::SeptimIconWidget()
    {
        _icon = _defaultIcon = ICON_FA_COINS;
    }

    std::string SeptimIconWidget::widgetConfigName() { return "SeptimIconWidget"; }
}
