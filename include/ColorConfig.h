#pragma once
#include "PCH.h"
#include "SKSEMenuFramework.h"

using ImGuiMCP::ImU32;

namespace InfoWidgets::ColorConfig
{
    ImGuiMCP::ImVec4 loadColorFromConfig(const toml::node_view<const toml::node> config, ImGuiMCP::ImVec4 defaultColor);
    void saveColorToConfig(ImGuiMCP::ImVec4 color, toml::table &config);
}
