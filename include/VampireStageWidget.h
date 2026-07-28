#pragma once

#include "BaseTextWidget.h"

namespace InfoWidgets
{
    enum class VampireStage
    {
        NotAVampire,
        Stage1,
        Stage2,
        Stage3,
        Stage4
    };

    class VampireStageMixin
    {
    protected:
        VampireStage vampireStage();

    private:
        RE::SpellItem *_abVampire01 = nullptr;
        RE::SpellItem *_abVampire02 = nullptr;
        RE::SpellItem *_abVampire03 = nullptr;
        RE::SpellItem *_abVampire04 = nullptr;
        bool _unavailable{false};
    };

    class VampireStageTextWidget : public TextWidget, public VampireStageMixin
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };

    class VampireStageIconWidget : public IconWidget, public VampireStageMixin
    {
    public:
        VampireStageIconWidget();
        std::string widgetConfigName() override;
        void update() override;
    };
}
