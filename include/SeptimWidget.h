#pragma once

#include "BaseTextWidget.h"

namespace InfoWidgets
{

    class SeptimTextWidget final : public TextWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };

    class SeptimIconWidget final : public IconWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };
}
