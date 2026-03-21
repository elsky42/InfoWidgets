#pragma once

#include <optional>

#include <PCH.h>
#include "ColorConfig.h"
#include "DrawUtils.h"


namespace InfoWidgets
{
    class Widget
    {
    public:
        virtual void configure(const toml::table &root) = 0;
        virtual bool renderConfig(toml::table &root) = 0;
        virtual std::string widgetConfigName() = 0;
        virtual void saveConfig(toml::table &root) = 0;
        virtual void updateAndRender(float deltaTime) = 0;
        virtual void disable() {}
    };

    // ---- Pure mixins (no Widget base – no diamond) ----
    // Each mixin receives the section name explicitly so it has no hidden
    // dependency on widgetConfigName() from a virtual base.

    class PollingMixin
    {
    public:
        void configure(const toml::table &root, const std::string &section)
        {
            if (root.empty())
                return;
            _enabled = root.at_path(section + ".enabled").value_or(false);
        }

        void saveConfig(toml::table &root, const std::string &section)
        {
            if (!root.contains(section))
                root.insert(section, toml::table{{"enabled", true}});
            root.get_as<toml::table>(section)->insert_or_assign("enabled", _enabled);
        }

        bool renderConfig()
        {
            return ImGuiMCP::ImGui::Checkbox("Enabled", &_enabled);
        }

    protected:
        bool _enabled{false};

    private:
        float _deltaTime{0.0f};

        // Tick returns true when an update is due (every 1 second).
        friend class BaseTextWidget;
        bool tick(float dt)
        {
            _deltaTime += dt;
            if (_deltaTime >= 1.0f)
            {
                _deltaTime = 0.0f;
                return true;
            }
            return false;
        }
    };

    class RectMixin
    {
    public:
        void configure(const toml::table &root, const std::string &section)
        {
            if (root.empty())
                return;
            auto x = root.at_path(section + ".position.x").value_or(0.0f);
            auto y = root.at_path(section + ".position.y").value_or(0.0f);
            _position = ImGuiMCP::ImVec2(x, y);
            _size = root.at_path(section + ".size").value_or(0.01f);
        }

        void saveConfig(toml::table &root, const std::string &section)
        {
            if (!root.contains(section))
                root.insert(section, toml::table{});
            auto &sec = *root.get_as<toml::table>(section);
            if (!sec.contains("position"))
                sec.insert("position", toml::table{});
            auto &pos = *sec.get_as<toml::table>("position");
            pos.insert_or_assign("x", _position.x);
            pos.insert_or_assign("y", _position.y);
            sec.insert_or_assign("size", _size);
        }

        bool renderConfig()
        {
            return ImGuiMCP::ImGui::DragFloat2("Position", &_position.x, 0.001f, 0.0f, 1.0f, "%.5f") ||
                   ImGuiMCP::ImGui::DragFloat("Size", &_size, 0.001f, 0.0f, 1.0f, "%.5f");
        }

        ImGuiMCP::ImVec2 normalizedPosition() const
        {
            return ImGuiMCP::ImVec2(DrawUtils::normalizeX(_position.x), DrawUtils::normalizeY(_position.y));
        }

        ImGuiMCP::ImVec2 normalizedShadowPosition() const
        {
            return ImGuiMCP::ImVec2(DrawUtils::normalizeX(_position.x) + 1.0f, DrawUtils::normalizeY(_position.y) + 1.0f);
        }

        float normalizedSize() const { return DrawUtils::normalizeY(_size); }

    protected:
        ImGuiMCP::ImVec2 _position{0.1f, 0.1f};
        float _size{0.1f};
    };

    class ColoredMixin
    {
    public:
        void configure(const toml::table &root, const std::string &section)
        {
            if (root.empty())
                return;
            _color = ColorConfig::loadColorFromConfig(root.at_path(section + ".color"), _color);
            _shadowColor = ColorConfig::loadColorFromConfig(root.at_path(section + ".shadowColor"), _shadowColor);
            _backgroundColor = ColorConfig::loadColorFromConfig(root.at_path(section + ".backgroundColor"), _backgroundColor);
            _outlineColor = ColorConfig::loadColorFromConfig(root.at_path(section + ".outlineColor"), _outlineColor);
        }

        void saveConfig(toml::table &root, const std::string &section)
        {
            if (!root.contains(section))
                root.insert(section, toml::table{});
            auto &sec = *root.get_as<toml::table>(section);
            if (!sec.contains("color"))
                sec.insert("color", toml::table{});
            if (!sec.contains("shadowColor"))
                sec.insert("shadowColor", toml::table{});
            if (!sec.contains("backgroundColor"))
                sec.insert("backgroundColor", toml::table{});
            if (!sec.contains("outlineColor"))
                sec.insert("outlineColor", toml::table{});
            ColorConfig::saveColorToConfig(_color, *sec.get_as<toml::table>("color"));
            ColorConfig::saveColorToConfig(_shadowColor, *sec.get_as<toml::table>("shadowColor"));
            ColorConfig::saveColorToConfig(_backgroundColor, *sec.get_as<toml::table>("backgroundColor"));
            ColorConfig::saveColorToConfig(_outlineColor, *sec.get_as<toml::table>("outlineColor"));
        }

        bool renderConfig()
        {
            return ImGuiMCP::ImGui::ColorEdit4("Color", &_color.x, ImGuiMCP::ImGuiColorEditFlags_Float) ||
                   ImGuiMCP::ImGui::ColorEdit4("Shadow Color", &_shadowColor.x, ImGuiMCP::ImGuiColorEditFlags_Float) ||
                   ImGuiMCP::ImGui::ColorEdit4("Background Color", &_backgroundColor.x, ImGuiMCP::ImGuiColorEditFlags_Float) ||
                   ImGuiMCP::ImGui::ColorEdit4("Outline Color", &_outlineColor.x, ImGuiMCP::ImGuiColorEditFlags_Float);
        }

        unsigned int color() const { return DrawUtils::rgbaToColor(_valueColor.value_or(_color)); }
        unsigned int shadowColor() const { return DrawUtils::rgbaToColor(_shadowColor); }
        unsigned int backgroundColor() const { return DrawUtils::rgbaToColor(_backgroundColor); }
        unsigned int outlineColor() const { return DrawUtils::rgbaToColor(_outlineColor); }

    protected:
        ImGuiMCP::ImVec4 _color{1.0f, 1.0f, 1.0f, 1.0f};
        ImGuiMCP::ImVec4 _shadowColor{0.0f, 0.0f, 0.0f, 0.0f};
        ImGuiMCP::ImVec4 _backgroundColor{0.0f, 0.0f, 0.0f, 0.0f};
        ImGuiMCP::ImVec4 _outlineColor{0.0f, 0.0f, 0.0f, 0.0f};
        std::optional<ImGuiMCP::ImVec4> _valueColor{std::nullopt};
    };

}