#include "NoiseWidget.h"

using namespace ImGuiMCP;

namespace InfoWidgets
{

    std::string NoiseTextWidget::widgetConfigName() { return "NoiseTextWidget"; }

    void NoiseTextWidget::configure(const toml::table &root)
    {
        TextWidget::configure(root);
        configureLevelColors(root, widgetConfigName());
    }

    void NoiseTextWidget::saveConfig(toml::table &root)
    {
        TextWidget::saveConfig(root);
        saveLevelColors(root, widgetConfigName());
    }

    bool NoiseTextWidget::renderConfig(toml::table &root)
    {
        bool changed = TextWidget::renderConfig(root);
        changed |= renderLevelColors();
        return changed;
    }

    void NoiseTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player || !player->IsSneaking())
        {
            _text = "";
            return;
        }
        const float noise = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMovementNoiseMult);
        const float noisePercent = std::ceil(noise * 100.0f);
        _text = std::format("{:.0f}", noisePercent);
        applyLevelColor(_valueColor, noisePercent);
    }

    std::string NoiseIconWidget::widgetConfigName() { return "NoiseIconWidget"; }

    void NoiseIconWidget::configure(const toml::table &root)
    {
        IconWidget::configure(root);
        configureLevelColors(root, widgetConfigName());
    }

    void NoiseIconWidget::saveConfig(toml::table &root)
    {
        IconWidget::saveConfig(root);
        saveLevelColors(root, widgetConfigName());
    }

    bool NoiseIconWidget::renderConfig(toml::table &root)
    {
        bool changed = IconWidget::renderConfig(root);
        changed |= renderLevelColors();
        return changed;
    }

    void NoiseIconWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player || !player->IsSneaking())
        {
            _text = "";
            return;
        }
        const float noise = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMovementNoiseMult);
        if (noise < 0)
        {
            _text = ICON_FA_VOLUME_OFF;
        }
        else if (noise < 0.25)
        {
            _text = ICON_FA_VOLUME_LOW;
        }
        else
        {
            _text = ICON_FA_VOLUME_HIGH;
        }
        applyLevelColor(_valueColor, std::ceil(noise * 100.0f));
    }
}