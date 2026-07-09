#include <format>

#include "D3DRenderer.h"
#include "GeneralConfig.h"

namespace InfoWidgets
{
    // ImGuiKey → DX scancode; also used in reverse by the sink to convert GetIDCode() → ImGuiKey
    static constexpr std::pair<ImGuiMCP::ImGuiKey, uint32_t> kKeyMap[] = {
        {ImGuiMCP::ImGuiKey_Tab, 15}, {ImGuiMCP::ImGuiKey_Backspace, 14},
        {ImGuiMCP::ImGuiKey_Space, 57}, {ImGuiMCP::ImGuiKey_Enter, 28},
        {ImGuiMCP::ImGuiKey_CapsLock, 58},
        // digits
        {ImGuiMCP::ImGuiKey_0, 11}, {ImGuiMCP::ImGuiKey_1,  2}, {ImGuiMCP::ImGuiKey_2,  3},
        {ImGuiMCP::ImGuiKey_3,  4}, {ImGuiMCP::ImGuiKey_4,  5}, {ImGuiMCP::ImGuiKey_5,  6},
        {ImGuiMCP::ImGuiKey_6,  7}, {ImGuiMCP::ImGuiKey_7,  8}, {ImGuiMCP::ImGuiKey_8,  9},
        {ImGuiMCP::ImGuiKey_9, 10},
        // letters
        {ImGuiMCP::ImGuiKey_A, 30}, {ImGuiMCP::ImGuiKey_B, 48}, {ImGuiMCP::ImGuiKey_C, 46},
        {ImGuiMCP::ImGuiKey_D, 32}, {ImGuiMCP::ImGuiKey_E, 18}, {ImGuiMCP::ImGuiKey_F, 33},
        {ImGuiMCP::ImGuiKey_G, 34}, {ImGuiMCP::ImGuiKey_H, 35}, {ImGuiMCP::ImGuiKey_I, 23},
        {ImGuiMCP::ImGuiKey_J, 36}, {ImGuiMCP::ImGuiKey_K, 37}, {ImGuiMCP::ImGuiKey_L, 38},
        {ImGuiMCP::ImGuiKey_M, 50}, {ImGuiMCP::ImGuiKey_N, 49}, {ImGuiMCP::ImGuiKey_O, 24},
        {ImGuiMCP::ImGuiKey_P, 25}, {ImGuiMCP::ImGuiKey_Q, 16}, {ImGuiMCP::ImGuiKey_R, 19},
        {ImGuiMCP::ImGuiKey_S, 31}, {ImGuiMCP::ImGuiKey_T, 20}, {ImGuiMCP::ImGuiKey_U, 22},
        {ImGuiMCP::ImGuiKey_V, 47}, {ImGuiMCP::ImGuiKey_W, 17}, {ImGuiMCP::ImGuiKey_X, 45},
        {ImGuiMCP::ImGuiKey_Y, 21}, {ImGuiMCP::ImGuiKey_Z, 44},
        // function keys
        {ImGuiMCP::ImGuiKey_F1,  59}, {ImGuiMCP::ImGuiKey_F2,  60}, {ImGuiMCP::ImGuiKey_F3,  61},
        {ImGuiMCP::ImGuiKey_F4,  62}, {ImGuiMCP::ImGuiKey_F5,  63}, {ImGuiMCP::ImGuiKey_F6,  64},
        {ImGuiMCP::ImGuiKey_F7,  65}, {ImGuiMCP::ImGuiKey_F8,  66}, {ImGuiMCP::ImGuiKey_F9,  67},
        {ImGuiMCP::ImGuiKey_F10, 68}, {ImGuiMCP::ImGuiKey_F11, 87}, {ImGuiMCP::ImGuiKey_F12, 88},
        // punctuation
        {ImGuiMCP::ImGuiKey_Apostrophe,   40}, {ImGuiMCP::ImGuiKey_Comma,        51},
        {ImGuiMCP::ImGuiKey_Minus,        12}, {ImGuiMCP::ImGuiKey_Period,       52},
        {ImGuiMCP::ImGuiKey_Slash,        53}, {ImGuiMCP::ImGuiKey_Semicolon,    39},
        {ImGuiMCP::ImGuiKey_Equal,        13}, {ImGuiMCP::ImGuiKey_LeftBracket,  26},
        {ImGuiMCP::ImGuiKey_Backslash,    43}, {ImGuiMCP::ImGuiKey_RightBracket, 27},
        {ImGuiMCP::ImGuiKey_GraveAccent,  41},
        // numpad
        {ImGuiMCP::ImGuiKey_Keypad0,        82}, {ImGuiMCP::ImGuiKey_Keypad1,      79},
        {ImGuiMCP::ImGuiKey_Keypad2,        80}, {ImGuiMCP::ImGuiKey_Keypad3,      81},
        {ImGuiMCP::ImGuiKey_Keypad4,        75}, {ImGuiMCP::ImGuiKey_Keypad5,      76},
        {ImGuiMCP::ImGuiKey_Keypad6,        77}, {ImGuiMCP::ImGuiKey_Keypad7,      71},
        {ImGuiMCP::ImGuiKey_Keypad8,        72}, {ImGuiMCP::ImGuiKey_Keypad9,      73},
        {ImGuiMCP::ImGuiKey_KeypadDecimal,  83}, {ImGuiMCP::ImGuiKey_KeypadMultiply, 55},
        {ImGuiMCP::ImGuiKey_KeypadSubtract, 74}, {ImGuiMCP::ImGuiKey_KeypadAdd,    78},
    };

    static ImGuiMCP::ImGuiKey dxScancodeToImGuiKey(uint32_t scancode)
    {
        for (auto [key, sc] : kKeyMap)
            if (sc == scancode)
                return key;
        return ImGuiMCP::ImGuiKey_None;
    }

    RE::BSEventNotifyControl GeneralConfig::Sink::ProcessEvent(
        RE::InputEvent *const *events,
        RE::BSTEventSource<RE::InputEvent *> *)
    {
        if (!events)
            return RE::BSEventNotifyControl::kContinue;

        for (auto *event = *events; event; event = event->next)
        {
            auto *button = event->AsButtonEvent();
            if (!button ||
                button->GetDevice() != RE::INPUT_DEVICE::kKeyboard ||
                !button->IsDown())
                continue;

            uint32_t tk = _owner._toggleKey.load();
            if (tk == 0)
                continue;

            auto pressed = dxScancodeToImGuiKey(button->GetIDCode());
            if (pressed != ImGuiMCP::ImGuiKey_None &&
                static_cast<uint32_t>(pressed) == tk)
                _owner._hudVisible.store(!_owner._hudVisible.load());
        }
        return RE::BSEventNotifyControl::kContinue;
    }

    void GeneralConfig::configure(const toml::table &root)
    {
        if (root.empty())
            return;
        _toggleKey.store(root.at_path("General.toggleKey").value_or(0u));
        _textFontPath = root.at_path("General.textFont").value_or(std::string{});
        _textFontSize = root.at_path("General.fontSize").value_or(20.0f);
        D3DRenderer::SetTextFontPath(_textFontPath);
        D3DRenderer::SetTextFontSize(_textFontSize);
    }

    void GeneralConfig::saveConfig(toml::table &root)
    {
        auto s = widgetConfigName();
        if (!root.contains(s))
            root.insert(s, toml::table{});
        auto &sec = *root.get_as<toml::table>(s);
        sec.insert_or_assign("toggleKey", _toggleKey.load());
        sec.insert_or_assign("textFont", _textFontPath);
        sec.insert_or_assign("fontSize", _textFontSize);
    }

    bool GeneralConfig::renderConfig(toml::table & /*root*/)
    {
        bool changed = false;
        if (_capturingKey.load())
        {
            ImGuiMCP::ImGui::Text("Press any key... (Escape to cancel)");

            if (ImGuiMCP::ImGui::IsKeyPressed(ImGuiMCP::ImGuiKey_Escape, false))
            {
                _capturingKey.store(false);
            }
            else
            {
                for (auto [imgKey, scancode] : kKeyMap)
                {
                    if (ImGuiMCP::ImGui::IsKeyPressed(imgKey, false))
                    {
                        _toggleKey.store(static_cast<uint32_t>(imgKey));
                        _capturingKey.store(false);
                        changed = true;
                        break;
                    }
                }
            }
        }
        else
        {
            uint32_t key = _toggleKey.load();
            const char *keyLabel = (key == 0)
                ? "Unbound"
                : ImGuiMCP::ImGui::GetKeyName(static_cast<ImGuiMCP::ImGuiKey>(key));
            std::string buttonLabel = std::format("Bind toggle key: {}", keyLabel);
            if (ImGuiMCP::ImGui::Button(buttonLabel.c_str()))
            {
                _capturingKey.store(true);
                changed = true;
            }
            ImGuiMCP::ImGui::SameLine();
            if (ImGuiMCP::ImGui::Button("Clear"))
            {
                _toggleKey.store(0);
                changed = true;
            }
        }
        ImGuiMCP::ImGui::Separator();

        const auto &fonts = D3DRenderer::AvailableFonts();
        int selected = -1; // -1 = default
        for (int i = 0; i < static_cast<int>(fonts.size()); ++i)
            if (fonts[i].path == _textFontPath) { selected = i; break; }

        const char *preview = (selected < 0) ? "(Default)" : fonts[selected].displayName.c_str();
        if (ImGuiMCP::ImGui::BeginCombo("Text Font", preview))
        {
            if (ImGuiMCP::ImGui::Selectable("(Default)", selected < 0))
            {
                _textFontPath = "";
                D3DRenderer::SetTextFontPath("");
                changed = true;
            }
            if (selected < 0) ImGuiMCP::ImGui::SetItemDefaultFocus();

            for (int i = 0; i < static_cast<int>(fonts.size()); ++i)
            {
                bool isSel = (i == selected);
                if (ImGuiMCP::ImGui::Selectable(fonts[i].displayName.c_str(), isSel))
                {
                    _textFontPath = fonts[i].path;
                    D3DRenderer::SetTextFontPath(_textFontPath);
                    changed = true;
                }
                if (isSel) ImGuiMCP::ImGui::SetItemDefaultFocus();
            }
            ImGuiMCP::ImGui::EndCombo();
        }

        if (ImGuiMCP::ImGui::DragFloat("Font Size", &_textFontSize, 0.5f, 8.0f, 64.0f, "%.0f"))
        {
            D3DRenderer::SetTextFontSize(_textFontSize);
            changed = true;
        }

        return changed;
    }
}
