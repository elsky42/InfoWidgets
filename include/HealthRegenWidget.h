#pragma once

#include "BaseTextWidget.h"

namespace InfoWidgets
{

    class HealthRegenTextWidget final : public TextWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };

    class HealthRegenIconWidget final : public IconWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };
}
