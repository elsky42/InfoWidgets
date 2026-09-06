#include <format>

#include "IconsFontAwesome6.h"

#include "TimeOfDayWidget.h"

namespace InfoWidgets
{
    std::string TimeOfDayTextWidget::widgetConfigName() { return "TimeOfDayTextWidget"; }

    void TimeOfDayTextWidget::update()
    {
        auto *calendar = RE::Calendar::GetSingleton();
        if (!calendar)
        {
            _text = "";
            return;
        }
        const int hour = static_cast<int>(calendar->GetHour());
        const std::uint32_t minutes = calendar->GetMinutes();
        _text = std::format("{}:{:02}", hour, minutes);
    }

    TimeOfDayIconWidget::TimeOfDayIconWidget()
    {
        _icon = _defaultIcon = ICON_FA_SUN;
    }

    std::string TimeOfDayIconWidget::widgetConfigName() { return "TimeOfDayIconWidget"; }

    void TimeOfDayIconWidget::configure(const toml::table &root)
    {
        IconWidget::configure(root);
        auto s = widgetConfigName();
        _dayColor = ColorConfig::loadColorFromConfig(root.at_path(s + ".dayColor"), _dayColor);
        _nightColor = ColorConfig::loadColorFromConfig(root.at_path(s + ".nightColor"), _nightColor);
    }

    void TimeOfDayIconWidget::saveConfig(toml::table &root)
    {
        IconWidget::saveConfig(root);
        auto &sec = *root.get_as<toml::table>(widgetConfigName());
        if (!sec.contains("dayColor"))
            sec.insert("dayColor", toml::table{});
        if (!sec.contains("nightColor"))
            sec.insert("nightColor", toml::table{});
        ColorConfig::saveColorToConfig(_dayColor, *sec.get_as<toml::table>("dayColor"));
        ColorConfig::saveColorToConfig(_nightColor, *sec.get_as<toml::table>("nightColor"));
    }

    bool TimeOfDayIconWidget::renderConfig(toml::table &root)
    {
        bool changed = IconWidget::renderConfig(root);
        changed |= ImGuiMCP::ImGui::ColorEdit4("Day Color", &_dayColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        changed |= ImGuiMCP::ImGui::ColorEdit4("Night Color", &_nightColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        return changed;
    }

    void TimeOfDayIconWidget::update()
    {
        auto *calendar = RE::Calendar::GetSingleton();
        if (!calendar)
        {
            _text = "";
            return;
        }
        const int hour = static_cast<int>(calendar->GetHour());
        const bool isDay = hour >= 5 && hour < 19;
        _text = isDay ? ICON_FA_SUN : ICON_FA_MOON;
        _valueColor = isDay ? _dayColor : _nightColor;
    }
}
