#include <cmath>
#include <format>

#include "IconsFontAwesome6.h"

#include "ChargeWidget.h"

namespace InfoWidgets
{
    std::optional<WeaponCharge> equippedWeaponCharge(bool isOffHand)
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
            return std::nullopt;

        auto *handItem = player->GetEquippedObject(isOffHand);
        if (!handItem)
            return std::nullopt;

        auto *weapon = handItem->As<RE::TESObjectWEAP>();
        if (!weapon)
            return std::nullopt;

        auto *enchantment = weapon->formEnchanting;
        float maxChargePoints = static_cast<float>(weapon->amountofEnchantment);
        float currentChargePoints = maxChargePoints;

        auto *entryData = player->GetEquippedEntryData(isOffHand);
        if (entryData && entryData->extraLists)
        {
            for (auto &xList : *entryData->extraLists)
            {
                if (!xList)
                    continue;
                auto *xCharge = xList->GetByType<RE::ExtraCharge>();
                auto *xEnch = xList->GetByType<RE::ExtraEnchantment>();
                if (xEnch && xEnch->enchantment && xEnch->charge != 0)
                {
                    enchantment = xEnch->enchantment;
                    maxChargePoints = static_cast<float>(xEnch->charge);
                    currentChargePoints = xCharge ? xCharge->charge : maxChargePoints;
                    break;
                }
                else if (xCharge && weapon->formEnchanting && weapon->amountofEnchantment != 0)
                {
                    currentChargePoints = xCharge->charge;
                    break;
                }
            }
        }

        if (!enchantment || maxChargePoints <= 0.0f)
            return std::nullopt;

        const float costPerUse = enchantment->CalculateMagickaCost(player);
        if (costPerUse <= 0.0f)
            return std::nullopt;

        WeaponCharge result;
        result.total = std::round(maxChargePoints / costPerUse);
        result.remaining = std::round(currentChargePoints / costPerUse);
        result.remaining = std::clamp(result.remaining, 0.0f, result.total);
        return result;
    }

    void ChargeMixin::applyChargeColor(std::optional<ImGuiMCP::ImVec4> &valueColor, float remaining, float total)
    {
        if (remaining <= 0.0f)
            valueColor = _emptyColor;
        else if (remaining <= total * 0.33f)
            valueColor = _lowColor;
        else
            valueColor = _fullColor;
    }

    void ChargeMixin::configureChargeColors(const toml::table &root, const std::string &section)
    {
        _fullColor = ColorConfig::loadColorFromConfig(root.at_path(section + ".fullColor"), _fullColor);
        _lowColor = ColorConfig::loadColorFromConfig(root.at_path(section + ".lowColor"), _lowColor);
        _emptyColor = ColorConfig::loadColorFromConfig(root.at_path(section + ".emptyColor"), _emptyColor);
    }

    void ChargeMixin::saveChargeColors(toml::table &root, const std::string &section)
    {
        auto &sec = *root.get_as<toml::table>(section);
        if (!sec.contains("fullColor"))
            sec.insert("fullColor", toml::table{});
        if (!sec.contains("lowColor"))
            sec.insert("lowColor", toml::table{});
        if (!sec.contains("emptyColor"))
            sec.insert("emptyColor", toml::table{});
        ColorConfig::saveColorToConfig(_fullColor, *sec.get_as<toml::table>("fullColor"));
        ColorConfig::saveColorToConfig(_lowColor, *sec.get_as<toml::table>("lowColor"));
        ColorConfig::saveColorToConfig(_emptyColor, *sec.get_as<toml::table>("emptyColor"));
    }

    bool ChargeMixin::renderChargeColors()
    {
        bool changed = ImGuiMCP::ImGui::ColorEdit4("Full Color", &_fullColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        changed |= ImGuiMCP::ImGui::ColorEdit4("Low Color", &_lowColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        changed |= ImGuiMCP::ImGui::ColorEdit4("Empty Color", &_emptyColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        return changed;
    }

    void ChargeTextWidget::configure(const toml::table &root)
    {
        TextWidget::configure(root);
        configureChargeColors(root, widgetConfigName());
    }

    void ChargeTextWidget::saveConfig(toml::table &root)
    {
        TextWidget::saveConfig(root);
        saveChargeColors(root, widgetConfigName());
    }

    bool ChargeTextWidget::renderConfig(toml::table &root)
    {
        bool changed = TextWidget::renderConfig(root);
        changed |= renderChargeColors();
        return changed;
    }

    void ChargeTextWidget::update()
    {
        auto charge = equippedWeaponCharge(_isOffHand);
        if (!charge)
        {
            _text = "";
            return;
        }
        _text = std::format("{:.0f} / {:.0f}", charge->remaining, charge->total);
        applyChargeColor(_valueColor, charge->remaining, charge->total);
    }

    std::string MainHandChargeTextWidget::widgetConfigName() { return "MainHandChargeTextWidget"; }

    OffHandChargeTextWidget::OffHandChargeTextWidget()
    {
        _isOffHand = true;
    }

    std::string OffHandChargeTextWidget::widgetConfigName() { return "OffHandChargeTextWidget"; }

    ChargeIconWidget::ChargeIconWidget()
    {
        _icon = _defaultIcon = ICON_FA_WAND_SPARKLES;
    }

    void ChargeIconWidget::configure(const toml::table &root)
    {
        IconWidget::configure(root);
        configureChargeColors(root, widgetConfigName());
    }

    void ChargeIconWidget::saveConfig(toml::table &root)
    {
        IconWidget::saveConfig(root);
        saveChargeColors(root, widgetConfigName());
    }

    bool ChargeIconWidget::renderConfig(toml::table &root)
    {
        bool changed = IconWidget::renderConfig(root);
        changed |= renderChargeColors();
        return changed;
    }

    void ChargeIconWidget::update()
    {
        auto charge = equippedWeaponCharge(_isOffHand);
        if (!charge)
        {
            _text = "";
            return;
        }
        _text = _icon;
        applyChargeColor(_valueColor, charge->remaining, charge->total);
    }

    std::string MainHandChargeIconWidget::widgetConfigName() { return "MainHandChargeIconWidget"; }

    OffHandChargeIconWidget::OffHandChargeIconWidget()
    {
        _isOffHand = true;
    }

    std::string OffHandChargeIconWidget::widgetConfigName() { return "OffHandChargeIconWidget"; }
}
