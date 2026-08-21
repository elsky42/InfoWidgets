#pragma once

#include <optional>

#include "BaseTextWidget.h"

namespace InfoWidgets
{
    struct WeaponCharge
    {
        float remaining;
        float total;
    };

    std::optional<WeaponCharge> equippedWeaponCharge(bool isOffHand);

    class ChargeMixin
    {
    protected:
        void applyChargeColor(std::optional<ImGuiMCP::ImVec4> &valueColor, float remaining, float total);
        void configureChargeColors(const toml::table &root, const std::string &section);
        void saveChargeColors(toml::table &root, const std::string &section);
        bool renderChargeColors();

        ImGuiMCP::ImVec4 _fullColor{1.0f, 1.0f, 1.0f, 1.0f};
        ImGuiMCP::ImVec4 _lowColor{1.0f, 0.85f, 0.0f, 1.0f};
        ImGuiMCP::ImVec4 _emptyColor{0.9f, 0.1f, 0.1f, 1.0f};
    };

    class ChargeTextWidget : public TextWidget, public ChargeMixin
    {
    public:
        void configure(const toml::table &root) override;
        void saveConfig(toml::table &root) override;
        bool renderConfig(toml::table &root) override;
        void update() override;

    protected:
        bool _isOffHand{false};
    };

    class MainHandChargeTextWidget final : public ChargeTextWidget
    {
    public:
        std::string widgetConfigName() override;
    };

    class OffHandChargeTextWidget final : public ChargeTextWidget
    {
    public:
        OffHandChargeTextWidget();
        std::string widgetConfigName() override;
    };

    class ChargeIconWidget : public IconWidget, public ChargeMixin
    {
    public:
        ChargeIconWidget();
        void configure(const toml::table &root) override;
        void saveConfig(toml::table &root) override;
        bool renderConfig(toml::table &root) override;
        void update() override;

    protected:
        bool _isOffHand{false};
    };

    class MainHandChargeIconWidget final : public ChargeIconWidget
    {
    public:
        std::string widgetConfigName() override;
    };

    class OffHandChargeIconWidget final : public ChargeIconWidget
    {
    public:
        OffHandChargeIconWidget();
        std::string widgetConfigName() override;
    };
}
