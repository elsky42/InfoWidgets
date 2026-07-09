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
