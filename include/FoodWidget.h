#pragma once
#include <optional>
#include <toml++/toml.hpp>
#include "SKSEMenuFramework.h"
#include "IconsFontAwesome6.h"

#include "ColorConfig.h"
#include "BaseTextWidget.h"

namespace InfoWidgets
{
    constexpr float oneMinute = 60.0f;
    constexpr float fiveMinutes = 300.0f;
    constexpr float fifteenMinutes = 900.0f;

    class FoodNeedWidget : public TextWidget
    {
    public:
        virtual RE::FormID keywordFormId() = 0;
        virtual std::string esp() = 0;
        virtual float lowThreshold() = 0;
        virtual float criticalThreshold() = 0;

        void configure(const toml::table &root) override;
        void saveConfig(toml::table &root) override;
        bool renderConfig(toml::table &root) override;
        void update() override;
        std::optional<float> getRemainingSeconds();

    private:
        ImGuiMCP::ImVec4 _lowColor{0.95f, 0.85f, 0.3f, 1.0f};
        ImGuiMCP::ImVec4 _criticalColor{0.95f, 0.85f, 0.3f, 1.0f};
        ImGuiMCP::ImVec4 _noneColor{0.9f, 0.2f, 0.2f, 1.0f};

        RE::BGSKeyword *_keyword{nullptr};
        bool _unavailable{false};
    };

    class MainCourseTextWidget : public FoodNeedWidget
    {
    public:
        MainCourseTextWidget();
        std::string widgetConfigName() override;
        RE::FormID keywordFormId() override;
        std::string esp() override;
        float lowThreshold() override;
        float criticalThreshold() override;
    };

    class SnackTextWidget : public FoodNeedWidget
    {
    public:
        SnackTextWidget();
        std::string widgetConfigName() override;
        RE::FormID keywordFormId() override;
        std::string esp() override;
        float lowThreshold() override;
        float criticalThreshold() override;
    };

    class DrinkTextWidget : public FoodNeedWidget
    {
    public:
        DrinkTextWidget();
        std::string widgetConfigName() override;
        RE::FormID keywordFormId() override;
        std::string esp() override;
        float lowThreshold() override;
        float criticalThreshold() override;
    };

    class FoodIconWidget : public IconWidget
    {
    public:
        FoodIconWidget();
        std::string widgetConfigName() override;
        void update() override;
    };
}
