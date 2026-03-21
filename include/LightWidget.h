#pragma once

#include "BaseTextWidget.h"

namespace InfoWidgets
{
    float GetPlayerLightLevel();

    class LightTextWidget final : public TextWidget
    {
        std::string widgetConfigName() override;
        void update() override;
    };

    class LightIconWidget final : public IconWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };
}