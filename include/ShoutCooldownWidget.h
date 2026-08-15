#pragma once

#include "BaseTextWidget.h"

namespace InfoWidgets
{
    class ShoutCooldownTextWidget final : public TextWidget
    {
    public:
        std::string widgetConfigName() override;
        void configure(const toml::table &root) override;
        void saveConfig(toml::table &root) override;
        bool renderConfig(toml::table &root) override;
        void update() override;

    protected:
        float pollingIntervalSeconds() const override { return 0.2f; }

    private:
        ImGuiMCP::ImVec4 _highCooldownColor{0.9f, 0.1f, 0.1f, 1.0f};
        ImGuiMCP::ImVec4 _mediumCooldownColor{1.0f, 0.85f, 0.0f, 1.0f};
        ImGuiMCP::ImVec4 _lowCooldownColor{1.0f, 0.95f, 0.6f, 1.0f};

        bool _hideWhenReady{false};
    };
}
