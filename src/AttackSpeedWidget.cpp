#include "AttackSpeedWidget.h"

using namespace ImGuiMCP;

namespace InfoWidgets
{
    std::optional<float> weaponAttackSpeed(bool isOffHand)
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
            return std::nullopt;
        auto *handItem = player->GetEquippedObject(isOffHand);
        if (handItem == nullptr)
            return std::nullopt;
        auto *weapon = handItem->As<RE::TESObjectWEAP>();
        if (weapon == nullptr)
            return std::nullopt;
        if (isOffHand && (weapon->IsTwoHandedAxe() || weapon->IsTwoHandedSword()))
            return std::nullopt;
        RE::ActorValue keyword = isOffHand ? RE::ActorValue::kLeftWeaponSpeedMultiply : RE::ActorValue::kWeaponSpeedMult;
        return weapon->GetSpeed() * player->AsActorValueOwner()->GetActorValue(keyword);
    }

    void AttackSpeedTextWidget::update()

    {
        auto weaponSpeed = weaponAttackSpeed(_isOffHand);
        if (weaponSpeed)
            _text = std::format("{:.2f}", *weaponSpeed);
        else
            _text = "";
    }

    std::string MainHandAttackSpeedTextWidget::widgetConfigName() { return "MainHandAttackSpeedTextWidget"; }

    OffHandAttackSpeedTextWidget::OffHandAttackSpeedTextWidget()
    {
        _isOffHand = true;
    }

    std::string OffHandAttackSpeedTextWidget::widgetConfigName() { return "OffHandAttackSpeedTextWidget"; }

    std::string MainHandAttackSpeedIconWidget::widgetConfigName() { return "MainHandAttackSpeedIconWidget"; }
    void MainHandAttackSpeedIconWidget::update()
    {
        _text = weaponAttackSpeed(false) ? "R speed" : "";
    }

    std::string OffHandAttackSpeedIconWidget::widgetConfigName() { return "OffHandAttackSpeedIconWidget"; }
    void OffHandAttackSpeedIconWidget::update()
    {
        _text = weaponAttackSpeed(true) ? "L speed" : "";
    }
}