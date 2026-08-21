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
            changed |= ColoredMixin::renderConfig(hasConfigurableColor());
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
        void renderConfigOverlay() override { RectMixin::renderConfigOverlay(); }
        float widgetSize() const override { return normalizedSize(); }

    protected:
        virtual void update() = 0;
        virtual float pollingIntervalSeconds() const { return 1.0f; }
        virtual bool hasConfigurableColor() const { return true; }
        void render();

        std::string _text;
        bool _wasEnabled{false};
    };

    class IconWidget : public BaseTextWidget
    {
    public:
        ImFont *font() override { return D3DRenderer::IconFont(); }

        void configure(const toml::table &root) override
        {
            BaseTextWidget::configure(root);
            if (!hasConfigurableIcon())
                return;
            if (!root.empty())
                _icon = root.at_path(widgetConfigName() + ".icon").value_or(_icon);
            _text = _icon;
        }

        void saveConfig(toml::table &root) override
        {
            BaseTextWidget::saveConfig(root);
            if (hasConfigurableIcon())
                root.get_as<toml::table>(widgetConfigName())->insert_or_assign("icon", _icon);
        }

        bool renderConfig(toml::table &root) override
        {
            bool changed = BaseTextWidget::renderConfig(root);
            if (hasConfigurableIcon())
                changed |= renderIconPicker();
            return changed;
        }

        void update() override { _text = _icon; }

    protected:
        virtual bool hasConfigurableIcon() const { return true; }
        std::string _icon;
        std::string _defaultIcon;

    private:
        char _iconFilter[64]{};
        bool renderIconPicker();
    };

    class TextWidget : public BaseTextWidget
    {
    public:
        ImFont *font() override { return D3DRenderer::TextFont(); }
    };
}
