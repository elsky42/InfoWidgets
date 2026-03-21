#pragma once

#include "BaseTextWidget.h"

namespace InfoWidgets
{
    class VampireStageTextWidget : public TextWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;

    private:
        RE::SpellItem *_abVampire01 = nullptr;
        RE::SpellItem *_abVampire02 = nullptr;
        RE::SpellItem *_abVampire03 = nullptr;
        RE::SpellItem *_abVampire04 = nullptr;
        bool _unavailable{false};
    };

    class VampireStageIconWidget : public IconWidget
    {
    public:
        VampireStageIconWidget();
        std::string widgetConfigName() override;
        void update() override;
    };
}
