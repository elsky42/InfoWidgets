#include "BaseTextWidget.h"

namespace InfoWidgets
{
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

        if (_backgroundColor.w > 0.0f)
        {
            constexpr float kPad = 2.0f;
            ImVec2 ts = f->CalcTextSizeA(size, FLT_MAX, 0.0f, _text.c_str());
            D3DRenderer::AddRectFilled(
                pos.x - kPad, pos.y - kPad,
                pos.x + ts.x + kPad, pos.y + ts.y + kPad,
                backgroundColor());
        }

        if (_outlineColor.w > 0.0f)
        {
            unsigned int oc = outlineColor();
            D3DRenderer::AddText(pos.x - 1.0f, pos.y, size, oc, f, _text.c_str());
            D3DRenderer::AddText(pos.x + 1.0f, pos.y, size, oc, f, _text.c_str());
            D3DRenderer::AddText(pos.x, pos.y - 1.0f, size, oc, f, _text.c_str());
            D3DRenderer::AddText(pos.x, pos.y + 1.0f, size, oc, f, _text.c_str());
        }

        if (_shadowColor.w > 0.0f)
        {
            auto shadow = normalizedShadowPosition();
            D3DRenderer::AddText(shadow.x, shadow.y, size, shadowColor(), f, _text.c_str());
        }

        D3DRenderer::AddText(pos.x, pos.y, size, color(), f, _text.c_str());
    }
}
