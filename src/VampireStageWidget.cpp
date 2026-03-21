#include "IconsFontAwesome6.h"

#include "VampireStageWidget.h"

using namespace ImGuiMCP;

namespace InfoWidgets
{
    std::string VampireStageTextWidget::widgetConfigName() { return "VampireStageTextWidget"; }

    void VampireStageTextWidget::update()
    {
        if (_unavailable)
            return;

        if (_abVampire01 == nullptr)
        {
            auto *handler = RE::TESDataHandler::GetSingleton();
            _abVampire01 = handler->LookupForm<RE::SpellItem>(0x02E1C3, "Skyrim.esm");
            _abVampire02 = handler->LookupForm<RE::SpellItem>(0x0ED099, "Skyrim.esm");
            _abVampire03 = handler->LookupForm<RE::SpellItem>(0x0ED09D, "Skyrim.esm");
            _abVampire04 = handler->LookupForm<RE::SpellItem>(0x0ED09E, "Skyrim.esm");
            if (!_abVampire01 || !_abVampire02 || !_abVampire03 || !_abVampire04)
            {
                SKSE::log::error("VampireStageTextWidget: failed to look up vampire spell forms from Skyrim.esm");
                _unavailable = true;
                return;
            }
        }

        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
            return;

        if (player->HasSpell(_abVampire04))
            _text = "4";
        else if (player->HasSpell(_abVampire03))
            _text = "3";
        else if (player->HasSpell(_abVampire02))
            _text = "2";
        else if (player->HasSpell(_abVampire01))
            _text = "1";
        else
            _text = "";
    }

    VampireStageIconWidget::VampireStageIconWidget()
    {
        _text = ICON_FA_DROPLET;
    }

    std::string VampireStageIconWidget::widgetConfigName()
    {
        return "VampireStageIconWidget";
    }

    void VampireStageIconWidget::update() {}
}
