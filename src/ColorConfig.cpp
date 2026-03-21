#include "ColorConfig.h"
#include "PCH.h"

using namespace ImGuiMCP;

namespace InfoWidgets::ColorConfig
{
    ImVec4 loadColorFromConfig(const toml::node_view<const toml::node> config, ImVec4 defaultColor)
    {
        if (!config)
            return defaultColor;
        return ImVec4(
            config.at_path("r").value_or(0.0f),
            config.at_path("g").value_or(0.0f),
            config.at_path("b").value_or(0.0f),
            config.at_path("a").value_or(0.0f));
    }

    void saveColorToConfig(ImVec4 color, toml::table &config)
    {
        config.insert_or_assign("r", color.x);
        config.insert_or_assign("g", color.y);
        config.insert_or_assign("b", color.z);
        config.insert_or_assign("a", color.w);
    }
}
