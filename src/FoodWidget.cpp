#include <optional>
#include <toml++/toml.hpp>
#include "IconsFontAwesome6.h"

#include "ColorConfig.h"
#include "FoodWidget.h"

namespace InfoWidgets
{

    void FoodNeedWidget::configure(const toml::table &root)
    {
        if (root.empty())
            return;
        TextWidget::configure(root);
        _lowColor = ColorConfig::loadColorFromConfig(root.at_path(widgetConfigName() + ".lowColor"), _lowColor);
        _criticalColor = ColorConfig::loadColorFromConfig(root.at_path(widgetConfigName() + ".criticalColor"), _criticalColor);
        _noneColor = ColorConfig::loadColorFromConfig(root.at_path(widgetConfigName() + ".noneColor"), _noneColor);
    }

    void FoodNeedWidget::saveConfig(toml::table &root)
    {
        TextWidget::saveConfig(root);

        auto &section = *root.get_as<toml::table>(widgetConfigName());
        if (!section.contains("lowColor"))
            section.insert("lowColor", toml::table{});
        if (!section.contains("criticalColor"))
            section.insert("criticalColor", toml::table{});
        if (!section.contains("noneColor"))
            section.insert("noneColor", toml::table{});
        ColorConfig::saveColorToConfig(_lowColor, *section.get_as<toml::table>("lowColor"));
        ColorConfig::saveColorToConfig(_criticalColor, *section.get_as<toml::table>("criticalColor"));
        ColorConfig::saveColorToConfig(_noneColor, *section.get_as<toml::table>("noneColor"));
    }

    bool FoodNeedWidget::renderConfig(toml::table &root)
    {
        bool changed = TextWidget::renderConfig(root);
        changed |= ImGuiMCP::ImGui::ColorEdit4("Low Color", &_lowColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        changed |= ImGuiMCP::ImGui::ColorEdit4("Critical Color", &_criticalColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        changed |= ImGuiMCP::ImGui::ColorEdit4("None Color", &_noneColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        return changed;
    }

    void FoodNeedWidget::update()
    {
        if (_unavailable)
            return;

        if (_keyword == nullptr)
        {
            auto *dataHandler = RE::TESDataHandler::GetSingleton();
            _keyword = dataHandler->LookupForm<RE::BGSKeyword>(keywordFormId(), esp());
            if (_keyword == nullptr)
            {
                SKSE::log::warn("{}: keyword not found in {}, widget will be hidden", widgetConfigName(), esp());
                _unavailable = true;
                return;
            }
        }
        auto remaining = getRemainingSeconds();
        if (remaining)
        {
            if (*remaining <= 0)
                _valueColor = std::nullopt;
            else if (*remaining <= criticalThreshold())
                _valueColor = _criticalColor;
            else if (*remaining <= lowThreshold())
                _valueColor = _lowColor;
            else
                _valueColor = _color;
        }
        else
        {
            _valueColor = _noneColor;
        }
    }

    std::optional<float> FoodNeedWidget::getRemainingSeconds()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
            return std::nullopt;
        auto *magicTarget = player->AsMagicTarget();
        if (!magicTarget)
            return std::nullopt;
        auto *activeEffects = magicTarget->GetActiveEffectList();
        if (!activeEffects)
            return std::nullopt;
        std::optional<float> remaining = std::nullopt;
        for (auto *effect : *activeEffects)
        {
            if (!effect || !effect->GetBaseObject())
                continue;
            auto *base = effect->GetBaseObject();
            if (base->HasKeyword(_keyword))
            {
                float duration = effect->duration;
                float elapsed = effect->elapsedSeconds;
                if (duration > 0.0f)
                {
                    remaining = duration - elapsed;
                    break;
                }
            }
        }
        return remaining;
    }

    MainCourseTextWidget::MainCourseTextWidget()
    {
        _text = "M";
    }
    std::string MainCourseTextWidget::widgetConfigName() { return "MainCourseTextWidget"; }
    RE::FormID MainCourseTextWidget::keywordFormId() { return 0x04C73F; }
    std::string MainCourseTextWidget::esp() { return "3tweaks.esp"; }
    float MainCourseTextWidget::lowThreshold() { return fifteenMinutes; }
    float MainCourseTextWidget::criticalThreshold() { return fiveMinutes; }

    SnackTextWidget::SnackTextWidget()
    {
        _text = "S";
    }
    std::string SnackTextWidget::widgetConfigName() { return "SnackTextWidget"; }
    RE::FormID SnackTextWidget::keywordFormId() { return 0x194D50; }
    std::string SnackTextWidget::esp() { return "ftweaks.esp"; }
    float SnackTextWidget::lowThreshold() { return fiveMinutes; }
    float SnackTextWidget::criticalThreshold() { return oneMinute; }

    DrinkTextWidget::DrinkTextWidget()
    {
        _text = "D";
    }
    std::string DrinkTextWidget::widgetConfigName() { return "DrinkTextWidget"; }
    RE::FormID DrinkTextWidget::keywordFormId() { return 0x000A01; }
    std::string DrinkTextWidget::esp() { return "ftweaks.esp"; }
    float DrinkTextWidget::lowThreshold() { return fiveMinutes; }
    float DrinkTextWidget::criticalThreshold() { return oneMinute; }

    FoodIconWidget::FoodIconWidget()
    {
        _text = ICON_FA_BOWL_FOOD;
    }
    std::string FoodIconWidget::widgetConfigName() { return "FoodIconWidget"; }
    void FoodIconWidget::update() {}
}
