#pragma once

#include "PCH.h"
#include "imgui.h"

namespace InfoWidgets::D3DRenderer
{
    void Install();

    // Register the per-frame HUD callback.
    void SetRenderCallback(void (*callback)());

    float GetDeltaTime();
    float GetDisplayWidth();
    float GetDisplayHeight();

    void AddText(float x, float y, float size, unsigned int color, ImFont *font, const char *text);
    void AddRectFilled(float x0, float y0, float x1, float y1, unsigned int color);

    ImFont *TextFont();
    ImFont *IconFont();

    struct FontEntry
    {
        std::string displayName;
        std::string path;
    };

    // Scans InfoWidgets/Fonts/ and Windows system fonts; result is cached.
    const std::vector<FontEntry>& AvailableFonts();

    // Must be called before the first frame (i.e. from LoadConfig).
    void SetTextFontPath(std::string path);
    void SetTextFontSize(float size);
}
