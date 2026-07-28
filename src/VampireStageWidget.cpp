#include "IconsFontAwesome6.h"

#include "VampireStageWidget.h"

using namespace ImGuiMCP;

namespace InfoWidgets
{
    VampireStage VampireStageMixin::vampireStage()
    {
        if (_unavailable)
            return VampireStage::NotAVampire;

        if (_abVampire01 == nullptr)
        {
            auto *handler = RE::TESDataHandler::GetSingleton();
            _abVampire01 = handler->LookupForm<RE::SpellItem>(0x02E1C3, "Skyrim.esm");
            _abVampire02 = handler->LookupForm<RE::SpellItem>(0x0ED099, "Skyrim.esm");
            _abVampire03 = handler->LookupForm<RE::SpellItem>(0x0ED09D, "Skyrim.esm");
            _abVampire04 = handler->LookupForm<RE::SpellItem>(0x0ED09E, "Skyrim.esm");
            if (!_abVampire01 || !_abVampire02 || !_abVampire03 || !_abVampire04)
            {
                SKSE::log::error("VampireStageMixin: failed to look up vampire spell forms from Skyrim.esm");
                _unavailable = true;
                return VampireStage::NotAVampire;
            }
        }

        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
            return VampireStage::NotAVampire;

        if (player->HasSpell(_abVampire04))
            return VampireStage::Stage4;
        if (player->HasSpell(_abVampire03))
            return VampireStage::Stage3;
        if (player->HasSpell(_abVampire02))
            return VampireStage::Stage2;
        if (player->HasSpell(_abVampire01))
            return VampireStage::Stage1;
        return VampireStage::NotAVampire;
    }

    std::string VampireStageTextWidget::widgetConfigName() { return "VampireStageTextWidget"; }

    void VampireStageTextWidget::update()
    {
        auto stage = vampireStage();
        _text = stage != VampireStage::NotAVampire
                    ? std::to_string(static_cast<int>(stage))
                    : "";
    }

    VampireStageIconWidget::VampireStageIconWidget()
    {
        _icon = _defaultIcon = ICON_FA_DROPLET;
    }

    std::string VampireStageIconWidget::widgetConfigName()
    {
        return "VampireStageIconWidget";
    }

    void VampireStageIconWidget::update()
    {
        _text = vampireStage() != VampireStage::NotAVampire ? _icon : "";
    }
}
