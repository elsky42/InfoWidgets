#pragma once

#include "BaseTextWidget.h"
#include "LevelColorMixin.h"

namespace InfoWidgets
{

    class NoiseTextWidget final : public TextWidget, public LevelColorMixin
    {
    public:
        std::string widgetConfigName() override;
        void configure(const toml::table &root) override;
        void saveConfig(toml::table &root) override;
        bool renderConfig(toml::table &root) override;
        void update() override;
    };

    class NoiseIconWidget final : public IconWidget, public LevelColorMixin
    {
    public:
        std::string widgetConfigName() override;
        void configure(const toml::table &root) override;
        void saveConfig(toml::table &root) override;
        bool renderConfig(toml::table &root) override;
        void update() override;

    protected:
        bool hasConfigurableIcon() const override { return false; }
    };
}