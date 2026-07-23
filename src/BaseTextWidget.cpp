#include "BaseTextWidget.h"

#include <cfloat>

#include "IconCatalog.h"

namespace InfoWidgets
{
    bool IconWidget::renderIconPicker()
    {
        bool changed = false;

        const char *currentName = "(Custom)";
        for (const auto &entry : IconCatalog::kEntries)
        {
            if (_icon == entry.glyph)
            {
                currentName = entry.name;
                break;
            }
        }

        if (ImGuiMCP::ImGui::BeginCombo("Icon", currentName, ImGuiMCP::ImGuiComboFlags_HeightLargest))
        {
            ImGuiMCP::ImGui::SetNextItemWidth(-1.0f);
            ImGuiMCP::ImGui::InputTextWithHint("##IconFilter", "Search icons...", _iconFilter, sizeof(_iconFilter));
            ImGuiMCP::ImGui::Separator();

            for (const auto &entry : IconCatalog::kEntries)
            {
                if (_iconFilter[0] != '\0' &&
                    !ImGuiMCP::ImGui::ImStristr(entry.name, nullptr, _iconFilter, nullptr))
                    continue;

                bool isSelected = (_icon == entry.glyph);
                if (ImGuiMCP::ImGui::Selectable(entry.name, isSelected))
                {
                    _icon = entry.glyph;
                    _text = _icon;
                    changed = true;
                }
                if (isSelected)
                    ImGuiMCP::ImGui::SetItemDefaultFocus();
            }
            ImGuiMCP::ImGui::EndCombo();
        }

        if (!_defaultIcon.empty() && _icon != _defaultIcon)
        {
            ImGuiMCP::ImGui::SameLine();
            if (ImGuiMCP::ImGui::Button("Reset to Default"))
            {
                _icon = _defaultIcon;
                _text = _icon;
                changed = true;
            }
        }

        return changed;
    }

    void BaseTextWidget::updateAndRender(float deltaTime)
    {
        if (!_enabled)
        {
            if (_wasEnabled)
            {
                _wasEnabled = false;
                disable();
            }
            return;
        }
        if (PollingMixin::tick(deltaTime))
            update();
        render();
        _wasEnabled = true;
    }

    void BaseTextWidget::render()
    {
        ImFont *f = font();
        if (!f)
            return;

        auto pos = normalizedPosition();
        float size = normalizedSize();

        if (_horizontalAlignment != HorizontalAlignment::Left && !_text.empty())
        {
            float textWidth = f->CalcTextSizeA(size, FLT_MAX, 0.0f, _text.c_str()).x;
            pos.x = applyHorizontalAlignment(pos.x, textWidth);
        }

        if (_outlineColor.w > 0.0f && _outlineSize > 0)
        {
            unsigned int oc = outlineColor();
            for (int dx = -_outlineSize; dx <= _outlineSize; ++dx)
                for (int dy = -_outlineSize; dy <= _outlineSize; ++dy)
                    if (dx != 0 || dy != 0)
                        D3DRenderer::AddText(pos.x + dx, pos.y + dy, size, oc, f, _text.c_str());
        }

        D3DRenderer::AddText(pos.x, pos.y, size, color(), f, _text.c_str());
    }
}
