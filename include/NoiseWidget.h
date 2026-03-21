#pragma once

#include "BaseTextWidget.h"

namespace InfoWidgets
{

    class NoiseTextWidget final : public TextWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };

    class NoiseIconWidget final : public IconWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };
}