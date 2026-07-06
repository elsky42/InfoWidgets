#pragma once

#include "BaseTextWidget.h"

namespace InfoWidgets
{

    class StaminaRegenTextWidget final : public TextWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };

    class StaminaRegenIconWidget final : public IconWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };
}
