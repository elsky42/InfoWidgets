#include "DrawUtils.h"
#include "D3DRenderer.h"

using ImGuiMCP::ImU32;

namespace InfoWidgets::DrawUtils
{
    float normalizeX(float x)
    {
        return x * D3DRenderer::GetDisplayWidth();
    }

    float normalizeY(float y)
    {
        return y * D3DRenderer::GetDisplayHeight();
    }

    unsigned int rgbaToColor(ImGuiMCP::ImVec4 color)
    {
        return IM_COL32(
            static_cast<int>(color.x * 255), static_cast<int>(color.y * 255),
            static_cast<int>(color.z * 255), static_cast<int>(color.w * 255));
    }
}
