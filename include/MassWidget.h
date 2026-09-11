#pragma once

#include "BaseTextWidget.h"

namespace InfoWidgets
{
    class MassTextWidget final : public TextWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };

    class MassIconWidget final : public IconWidget
    {
    public:
        MassIconWidget();
        std::string widgetConfigName() override;
    };
}
