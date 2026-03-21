#pragma once

#include <optional>

#include "BaseTextWidget.h"

namespace InfoWidgets
{
    std::optional<float> weaponAttackSpeed(bool isOffHand);

    class AttackSpeedTextWidget : public TextWidget
    {
    public:
        void update() override;

    protected:
        bool _isOffHand{false};
    };

    class MainHandAttackSpeedTextWidget : public AttackSpeedTextWidget
    {
    public:
        std::string widgetConfigName() override;
    };

    class OffHandAttackSpeedTextWidget : public AttackSpeedTextWidget
    {
    public:
        OffHandAttackSpeedTextWidget();
        std::string widgetConfigName() override;
    };

    class MainHandAttackSpeedIconWidget : public IconWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };

    class OffHandAttackSpeedIconWidget : public IconWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };
}