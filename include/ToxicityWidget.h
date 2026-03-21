#pragma once

#include "BaseTextWidget.h"

namespace InfoWidgets
{
    class ToxicityTextWidget : public TextWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;

    private:
        RE::TESGlobal *_toxicitySlotsAvailable = nullptr;
        bool _unavailable{false};
    };

    class ToxicityIconWidget : public IconWidget
    {
    public:
        ToxicityIconWidget();
        std::string widgetConfigName() override;
        void update() override;
    };
}