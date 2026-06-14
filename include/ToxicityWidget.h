#pragma once

#include "ColorConfig.h"
#include "BaseTextWidget.h"

namespace InfoWidgets
{
    class ToxicityColorMixin
    {
    protected:
        bool fetchToxicityData(float &used, float &max, const std::string &widgetName);
        void applyToxicityColor(std::optional<ImGuiMCP::ImVec4> &valueColor, float used, float max);
        void configureToxicityColors(const toml::table &root, const std::string &section);
        void saveToxicityColors(toml::table &root, const std::string &section);
        bool renderToxicityColors();

        ImGuiMCP::ImVec4 _warningColor{1.0f, 0.85f, 0.0f, 1.0f};
        ImGuiMCP::ImVec4 _dangerColor{0.9f, 0.1f, 0.1f, 1.0f};
        ImGuiMCP::ImVec4 _overdoseColor{0.6f, 0.0f, 0.9f, 1.0f};

    private:
        RE::TESGlobal *_toxicitySlotsAvailable{nullptr};
        bool _unavailable{false};
    };

    class ToxicityTextWidget : public TextWidget, public ToxicityColorMixin
    {
    public:
        std::string widgetConfigName() override;
        void configure(const toml::table &root) override;
        void saveConfig(toml::table &root) override;
        bool renderConfig(toml::table &root) override;
        void update() override;
    };

    class ToxicityIconWidget : public IconWidget, public ToxicityColorMixin
    {
    public:
        ToxicityIconWidget();
        std::string widgetConfigName() override;
        void configure(const toml::table &root) override;
        void saveConfig(toml::table &root) override;
        bool renderConfig(toml::table &root) override;
        void update() override;
    };
}
