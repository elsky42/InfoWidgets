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
        if (!player) { _text = ""; return; }
        _text = std::format("{:.0f}", GetResist(RE::ActorValue::kResistMagic));
    }

    std::string MagicResistIconWidget::widgetConfigName() { return "MagicResistIconWidget"; }

    void MagicResistIconWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) { _text = ""; return; }
        _text = ICON_FA_HAT_WIZARD;
    }

    // --- Fire Resist ---

    std::string FireResistTextWidget::widgetConfigName() { return "FireResistTextWidget"; }

    void FireResistTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) { _text = ""; return; }
        _text = std::format("{:.0f}", GetResist(RE::ActorValue::kResistFire));
    }

    std::string FireResistIconWidget::widgetConfigName() { return "FireResistIconWidget"; }

    void FireResistIconWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) { _text = ""; return; }
        _text = ICON_FA_FIRE;
    }

    // --- Frost Resist ---

    std::string FrostResistTextWidget::widgetConfigName() { return "FrostResistTextWidget"; }

    void FrostResistTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) { _text = ""; return; }
        _text = std::format("{:.0f}", GetResist(RE::ActorValue::kResistFrost));
    }

    std::string FrostResistIconWidget::widgetConfigName() { return "FrostResistIconWidget"; }

    void FrostResistIconWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) { _text = ""; return; }
        _text = ICON_FA_SNOWFLAKE;
    }

    // --- Shock Resist ---

    std::string ShockResistTextWidget::widgetConfigName() { return "ShockResistTextWidget"; }

    void ShockResistTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) { _text = ""; return; }
        _text = std::format("{:.0f}", GetResist(RE::ActorValue::kResistShock));
    }

    std::string ShockResistIconWidget::widgetConfigName() { return "ShockResistIconWidget"; }

    void ShockResistIconWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) { _text = ""; return; }
        _text = ICON_FA_BOLT_LIGHTNING;
    }

    // --- Poison Resist ---

    std::string PoisonResistTextWidget::widgetConfigName() { return "PoisonResistTextWidget"; }

    void PoisonResistTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) { _text = ""; return; }
        _text = std::format("{:.0f}", GetResist(RE::ActorValue::kPoisonResist));
    }

    std::string PoisonResistIconWidget::widgetConfigName() { return "PoisonResistIconWidget"; }

    void PoisonResistIconWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) { _text = ""; return; }
        _text = ICON_FA_SKULL_CROSSBONES;
    }

    // --- Disease Resist ---

    std::string DiseaseResistTextWidget::widgetConfigName() { return "DiseaseResistTextWidget"; }

    void DiseaseResistTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) { _text = ""; return; }
        _text = std::format("{:.0f}", GetResist(RE::ActorValue::kResistDisease));
    }

    std::string DiseaseResistIconWidget::widgetConfigName() { return "DiseaseResistIconWidget"; }

    void DiseaseResistIconWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) { _text = ""; return; }
        _text = ICON_FA_VIRUS;
    }

    // --- Armor Rating ---

    std::string ArmorRatingTextWidget::widgetConfigName() { return "ArmorRatingTextWidget"; }

    void ArmorRatingTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) { _text = ""; return; }
        _text = std::format("{:.0f}", GetResist(RE::ActorValue::kDamageResist));
    }

    std::string ArmorRatingIconWidget::widgetConfigName() { return "ArmorRatingIconWidget"; }

    void ArmorRatingIconWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) { _text = ""; return; }
        _text = ICON_FA_SHIELD;
    }
}
