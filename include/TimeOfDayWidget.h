#pragma once

#include "BaseTextWidget.h"

namespace InfoWidgets
{
    class TimeOfDayTextWidget final : public TextWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };

    class TimeOfDayIconWidget final : public IconWidget
    {
    public:
        TimeOfDayIconWidget();
        std::string widgetConfigName() override;
        void configure(const toml::table &root) override;
        void saveConfig(toml::table &root) override;
        bool renderConfig(toml::table &root) override;
        void update() override;

    protected:
        bool hasConfigurableIcon() const override { return false; }
        bool hasConfigurableColor() const override { return false; }

    private:
        ImGuiMCP::ImVec4 _dayColor{1.0f, 1.0f, 1.0f, 1.0f};
        ImGuiMCP::ImVec4 _nightColor{1.0f, 1.0f, 1.0f, 1.0f};
    };
}
