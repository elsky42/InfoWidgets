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

    SeptimIconWidget::SeptimIconWidget()
    {
        _icon = _defaultIcon = ICON_FA_COINS;
    }

    std::string SeptimIconWidget::widgetConfigName() { return "SeptimIconWidget"; }
}
