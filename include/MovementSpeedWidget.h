#pragma once

#include "BaseTextWidget.h"

namespace InfoWidgets
{

    class MovementSpeedTextWidget final : public TextWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };

    class MovementSpeedIconWidget final : public IconWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };
}
