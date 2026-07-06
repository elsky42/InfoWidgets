#pragma once

#include "BaseTextWidget.h"

namespace InfoWidgets
{

    class MagickaRegenTextWidget final : public TextWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };

    class MagickaRegenIconWidget final : public IconWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };
}
