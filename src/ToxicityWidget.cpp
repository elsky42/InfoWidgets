#include <format>

#include "IconsFontAwesome6.h"

#include "ToxicityWidget.h"

namespace InfoWidgets
{
    std::string ToxicityTextWidget::widgetConfigName() { return "ToxicityTextWidget"; }

    void ToxicityTextWidget::update()
    {
        if (_unavailable)
            return;

        if (!_toxicitySlotsAvailable)
        {
            auto toxicitySlotsAvailableForm = RE::TESForm::LookupByEditorID("ToxicitySlots_Forn");
            if (toxicitySlotsAvailableForm)
                _toxicitySlotsAvailable = toxicitySlotsAvailableForm->As<RE::TESGlobal>();

            if (!_toxicitySlotsAvailable)
            {
                SKSE::log::warn("ToxicityTextWidget: global variable ToxicitySlots_Forn not found, widget will be hidden");
                _unavailable = true;
                _text = "";
                return;
            }
        }

        auto player = RE::PlayerCharacter::GetSingleton();
        if (!player)
            return;
        auto toxicitySlotsUsed = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kVariable05);

        _text = std::format("{} / {}", toxicitySlotsUsed, _toxicitySlotsAvailable->value);
    }

    ToxicityIconWidget::ToxicityIconWidget()
    {
        _text = ICON_FA_BOTTLE_DROPLET;
    }

    std::string ToxicityIconWidget::widgetConfigName() { return "ToxicityIconWidget"; }

    void ToxicityIconWidget::update() {}
}