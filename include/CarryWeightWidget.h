#pragma once

#include "ColorConfig.h"
#include "BaseTextWidget.h"

namespace InfoWidgets
{
    class CarryWeightColorMixin
    {
    protected:
        void applyCarryWeightColor(std::optional<ImGuiMCP::ImVec4> &valueColor, float current, float max);
        void configureCarryWeightColors(const toml::table &root, const std::string &section);
        void saveCarryWeightColors(toml::table &root, const std::string &section);
        bool renderCarryWeightColors();

        ImGuiMCP::ImVec4 _warningColor{1.0f, 0.85f, 0.0f, 1.0f};
        ImGuiMCP::ImVec4 _criticalColor{0.9f, 0.1f, 0.1f, 1.0f};
    };

    class CarryWeightTextWidget : public TextWidget, public CarryWeightColorMixin
    {
    public:
        std::string widgetConfigName() override;
        void configure(const toml::table &root) override;
        void saveConfig(toml::table &root) override;
        bool renderConfig(toml::table &root) override;
        void update() override;
    };

    class CarryWeightIconWidget : public IconWidget, public CarryWeightColorMixin
    {
    public:
        CarryWeightIconWidget();
        std::string widgetConfigName() override;
        void configure(const toml::table &root) override;
        void saveConfig(toml::table &root) override;
        bool renderConfig(toml::table &root) override;
        void update() override;
    };
}
