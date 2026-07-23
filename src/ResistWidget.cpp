#include "ResistWidget.h"

using namespace ImGuiMCP;

namespace InfoWidgets
{

    static float GetResist(RE::ActorValue av)
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
            return 0.0f;
        return player->AsActorValueOwner()->GetActorValue(av);
    }

    // --- Magic Resist ---

    std::string MagicResistTextWidget::widgetConfigName() { return "MagicResistTextWidget"; }

    void MagicResistTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
        {
            _text = "";
            return;
        }
        _text = std::format("{:.0f}", GetResist(RE::ActorValue::kResistMagic));
    }

    MagicResistIconWidget::MagicResistIconWidget()
    {
        _icon = _defaultIcon = ICON_FA_HAT_WIZARD;
    }

    std::string MagicResistIconWidget::widgetConfigName() { return "MagicResistIconWidget"; }

    // --- Fire Resist ---

    std::string FireResistTextWidget::widgetConfigName() { return "FireResistTextWidget"; }

    void FireResistTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
        {
            _text = "";
            return;
        }
        _text = std::format("{:.0f}", GetResist(RE::ActorValue::kResistFire));
    }

    FireResistIconWidget::FireResistIconWidget()
    {
        _icon = _defaultIcon = ICON_FA_FIRE;
    }

    std::string FireResistIconWidget::widgetConfigName() { return "FireResistIconWidget"; }

    // --- Frost Resist ---

    std::string FrostResistTextWidget::widgetConfigName() { return "FrostResistTextWidget"; }

    void FrostResistTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
        {
            _text = "";
            return;
        }
        _text = std::format("{:.0f}", GetResist(RE::ActorValue::kResistFrost));
    }

    FrostResistIconWidget::FrostResistIconWidget()
    {
        _icon = _defaultIcon = ICON_FA_SNOWFLAKE;
    }

    std::string FrostResistIconWidget::widgetConfigName() { return "FrostResistIconWidget"; }

    // --- Shock Resist ---

    std::string ShockResistTextWidget::widgetConfigName() { return "ShockResistTextWidget"; }

    void ShockResistTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
        {
            _text = "";
            return;
        }
        _text = std::format("{:.0f}", GetResist(RE::ActorValue::kResistShock));
    }

    ShockResistIconWidget::ShockResistIconWidget()
    {
        _icon = _defaultIcon = ICON_FA_BOLT_LIGHTNING;
    }

    std::string ShockResistIconWidget::widgetConfigName() { return "ShockResistIconWidget"; }

    // --- Poison Resist ---

    std::string PoisonResistTextWidget::widgetConfigName() { return "PoisonResistTextWidget"; }

    void PoisonResistTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
        {
            _text = "";
            return;
        }
        _text = std::format("{:.0f}", GetResist(RE::ActorValue::kPoisonResist));
    }

    PoisonResistIconWidget::PoisonResistIconWidget()
    {
        _icon = _defaultIcon = ICON_FA_SKULL_CROSSBONES;
    }

    std::string PoisonResistIconWidget::widgetConfigName() { return "PoisonResistIconWidget"; }

    // --- Disease Resist ---

    std::string DiseaseResistTextWidget::widgetConfigName() { return "DiseaseResistTextWidget"; }

    void DiseaseResistTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
        {
            _text = "";
            return;
        }
        _text = std::format("{:.0f}", GetResist(RE::ActorValue::kResistDisease));
    }

    DiseaseResistIconWidget::DiseaseResistIconWidget()
    {
        _icon = _defaultIcon = ICON_FA_VIRUS;
    }

    std::string DiseaseResistIconWidget::widgetConfigName() { return "DiseaseResistIconWidget"; }

    // --- Armor Rating ---

    std::string ArmorRatingTextWidget::widgetConfigName() { return "ArmorRatingTextWidget"; }

    void ArmorRatingTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
        {
            _text = "";
            return;
        }
        _text = std::format("{:.0f}", GetResist(RE::ActorValue::kDamageResist));
    }

    ArmorRatingIconWidget::ArmorRatingIconWidget()
    {
        _icon = _defaultIcon = ICON_FA_SHIELD;
    }

    std::string ArmorRatingIconWidget::widgetConfigName() { return "ArmorRatingIconWidget"; }
}
