#pragma once

#include "BaseTextWidget.h"

namespace InfoWidgets
{

    class SpellPowerTextWidget final : public TextWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };

    class SpellPowerIconWidget final : public IconWidget
    {
    public:
        SpellPowerIconWidget();
        std::string widgetConfigName() override;
    };
}
