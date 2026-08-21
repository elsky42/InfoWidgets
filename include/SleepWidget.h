#pragma once

#include "BaseTextWidget.h"

namespace InfoWidgets
{
    enum class SleepLevel
    {
        Werewolf,
        NotRested,
        Rested,
        WellRested,
        LoversComfort
    };

    class SleepIconWidget final : public IconWidget
    {
    public:
        SleepIconWidget();
        std::string widgetConfigName() override;
        void configure(const toml::table &root) override;
        void saveConfig(toml::table &root) override;
        bool renderConfig(toml::table &root) override;
        void update() override;

    private:
        SleepLevel sleepLevel();

        RE::SpellItem *_restedSpell = nullptr;
        RE::SpellItem *_wellRestedSpell = nullptr;
        RE::SpellItem *_loversComfortSpell = nullptr;
        RE::TESGlobal *_werewolfGlobal = nullptr;
        bool _unavailable{false};

        ImGuiMCP::ImVec4 _notRestedColor{0.9f, 0.1f, 0.1f, 1.0f};
        ImGuiMCP::ImVec4 _restedColor{1.0f, 0.85f, 0.0f, 1.0f};
        ImGuiMCP::ImVec4 _wellRestedColor{1.0f, 1.0f, 1.0f, 1.0f};
        ImGuiMCP::ImVec4 _loversComfortColor{0.25f, 0.88f, 0.82f, 1.0f};
        ImGuiMCP::ImVec4 _werewolfColor{1.0f, 1.0f, 1.0f, 1.0f};

        bool _hideWhenWellRested{false};
        bool _hideWhenWerewolf{false};
    };
}
