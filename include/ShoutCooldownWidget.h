#pragma once

#include "BaseTextWidget.h"

namespace InfoWidgets
{
    class ShoutCooldownMixin
    {
    protected:
        float remainingShoutCooldown() const;
        void applyCooldownColor(std::optional<ImGuiMCP::ImVec4> &valueColor, float remaining);
        void configureCooldownColors(const toml::table &root, const std::string &section);
        void saveCooldownColors(toml::table &root, const std::string &section);
        bool renderCooldownColors();

        bool hideWhenReady() const { return _hideWhenReady; }
        void configureHideWhenReady(const toml::table &root, const std::string &section);
        void saveHideWhenReady(toml::table &root, const std::string &section);
        bool renderHideWhenReady();

        bool requireShoutSelected() const { return _requireShoutSelected; }
        bool isShoutSelected() const;
        void configureRequireShoutSelected(const toml::table &root, const std::string &section);
        void saveRequireShoutSelected(toml::table &root, const std::string &section);
        bool renderRequireShoutSelected();

        ImGuiMCP::ImVec4 _highCooldownColor{0.9f, 0.1f, 0.1f, 1.0f};
        ImGuiMCP::ImVec4 _mediumCooldownColor{1.0f, 0.85f, 0.0f, 1.0f};
        ImGuiMCP::ImVec4 _lowCooldownColor{1.0f, 0.95f, 0.6f, 1.0f};

    private:
        bool _hideWhenReady{false};
        bool _requireShoutSelected{true};
    };

    class ShoutCooldownTextWidget final : public TextWidget, public ShoutCooldownMixin
    {
    public:
        std::string widgetConfigName() override;
        void configure(const toml::table &root) override;
        void saveConfig(toml::table &root) override;
        bool renderConfig(toml::table &root) override;
        void update() override;

    protected:
        float pollingIntervalSeconds() const override { return 0.2f; }
    };

    class ShoutCooldownIconWidget final : public IconWidget, public ShoutCooldownMixin
    {
    public:
        ShoutCooldownIconWidget();
        std::string widgetConfigName() override;
        void configure(const toml::table &root) override;
        void saveConfig(toml::table &root) override;
        bool renderConfig(toml::table &root) override;
        void update() override;

    protected:
        float pollingIntervalSeconds() const override { return 0.2f; }
    };
}
