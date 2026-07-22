#include "SeptimWidget.h"

using namespace ImGuiMCP;

namespace InfoWidgets
{

    std::string SeptimTextWidget::widgetConfigName() { return "SeptimTextWidget"; }

    void SeptimTextWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) { _text = ""; return; }
        _text = std::format("{}", player->GetGoldAmount());
    }

    std::string SeptimIconWidget::widgetConfigName() { return "SeptimIconWidget"; }

    void SeptimIconWidget::update()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) { _text = ""; return; }
        _text = ICON_FA_COINS;
    }
}
