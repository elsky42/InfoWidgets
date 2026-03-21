#pragma once

#include "PCH.h"

namespace InfoWidgets::DrawUtils
{
    float normalizeX(float x);
    float normalizeY(float y);
    unsigned int rgbaToColor(ImGuiMCP::ImVec4 color);
}
