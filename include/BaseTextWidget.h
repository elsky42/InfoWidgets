#pragma once

#include "D3DRenderer.h"
#include "Widget.h"

namespace InfoWidgets
{
    class BaseTextWidget : public Widget, public PollingMixin, public RectMixin, public ColoredMixin
    {
    public:
        virtual std::string widgetConfigName() = 0;
        virtual ImFont *font() = 0;

        void configure(const toml::table &root) override
        {
            if (root.empty())
                return;
            auto s = widgetConfigName();
            PollingMixin::configure(root, s);
            RectMixin::configure(root, s);
            ColoredMixin::configure(root, s);
        }

        bool renderConfig(toml::table & /*root*/) override
        {
            bool changed = PollingMixin::renderConfig();
            changed |= RectMixin::renderConfig();
            changed |= ColoredMixin::renderConfig();
            return changed;
        }

        void saveConfig(toml::table &root) override
        {
            auto s = widgetConfigName();
            if (!root.contains(s))
                root.insert(s, toml::table{
                                   {"position", toml::table{}},
                                   {"color", toml::table{}},
                                   {"shadowColor", toml::table{}}});
            PollingMixin::saveConfig(root, s);
            RectMixin::saveConfig(root, s);
            ColoredMixin::saveConfig(root, s);
        }

        void updateAndRender(float deltaTime) override final;

    protected:
        virtual void update() = 0;
        void render();

        std::string _text;
        bool _wasEnabled{false};
    };

    class IconWidget : public BaseTextWidget
    {
    public:
        ImFont *font() override { return D3DRenderer::IconFont(); }
    };

    class TextWidget : public BaseTextWidget
    {
    public:
        ImFont *font() override { return D3DRenderer::TextFont(); }
    };
}
