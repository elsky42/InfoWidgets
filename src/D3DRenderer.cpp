#include "D3DRenderer.h"

#include "imgui_impl_dx11.h"

namespace InfoWidgets::D3DRenderer
{
    static ImGuiContext *g_imguiContext = nullptr;
    static bool g_initialized = false;
    static void (*g_renderCallback)() = nullptr;

    static LARGE_INTEGER g_lastTime{};
    static LARGE_INTEGER g_freq{};

    static float g_displayW = 1920.0f;
    static float g_displayH = 1080.0f;
    static float g_deltaTime = 0.016f;

    static ImFont *font_text = nullptr;
    static ImFont *font_icon = nullptr;

    static void TryLoadJost(ImGuiIO &io, float fontSize)
    {
        const char *path = "Data/SKSE/Plugins/InfoWidgets/Fonts/static/Jost-Regular.ttf";
        if (!std::filesystem::exists(path))
        {
            SKSE::log::error("TryLoadJost: font file does not exist at path: {}", path);
            return;
        }
        ImFontConfig cfg{};
        cfg.PixelSnapH = true;
        cfg.OversampleH = 1;
        cfg.OversampleV = 1;

        font_text = io.Fonts->AddFontFromFileTTF(path, fontSize, &cfg, nullptr);
        if (!font_text)
            SKSE::log::error("TryLoadJost: failed to load Jost font from '{}'", path);
    }

    static void TryLoadFontAwesome(ImGuiIO &io, float fontSize)
    {
        if (!font_text)
        {
            SKSE::log::error("TryLoadFontAwesome: cannot merge icons, font_text is null");
            return;
        }

        const char *path = "Data/SKSE/Plugins/InfoWidgets/Fonts/fa-solid-900.ttf";
        if (!std::filesystem::exists(path))
        {
            SKSE::log::warn("D3DRenderer: Font Awesome not found at '{}', icon widgets will be invisible", path);
            return;
        }

        static const ImWchar faRanges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
        ImFontConfig cfg{};
        cfg.PixelSnapH = true;
        cfg.OversampleH = 1;
        cfg.OversampleV = 1;
        cfg.MergeMode = true;

        font_icon = io.Fonts->AddFontFromFileTTF(path, fontSize, &cfg, faRanges);
        if (font_icon)
            SKSE::log::info("D3DRenderer: loaded Font Awesome");
        else
            SKSE::log::warn("D3DRenderer: Font Awesome not found, icon widgets will be invisible");
    }

    static bool Init()
    {
        auto *rd = RE::BSGraphics::Renderer::GetRendererData();
        if (!rd)
            return false;

        auto *device = reinterpret_cast<ID3D11Device *>(rd->forwarder);
        auto *ctx = reinterpret_cast<ID3D11DeviceContext *>(rd->context);
        if (!device || !ctx)
            return false;

        IMGUI_CHECKVERSION();
        g_imguiContext = ImGui::CreateContext();
        ImGui::SetCurrentContext(g_imguiContext);

        ImGuiIO &io = ImGui::GetIO();
        io.IniFilename = nullptr;
        io.LogFilename = nullptr;
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
        io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

        constexpr float kFontSize = 20.0f;
        TryLoadJost(io, kFontSize);
        TryLoadFontAwesome(io, kFontSize);
        if (!font_text)
        {
            SKSE::log::warn("D3DRenderer: falling back to built-in ProggyClean for text font");
            font_text = io.Fonts->AddFontDefault();
        }
        io.Fonts->Build();

        if (!ImGui_ImplDX11_Init(device, ctx))
        {
            SKSE::log::error("D3DRenderer::Init: ImGui_ImplDX11_Init failed");
            ImGui::DestroyContext(g_imguiContext);
            g_imguiContext = nullptr;
            return false;
        }

        QueryPerformanceFrequency(&g_freq);
        QueryPerformanceCounter(&g_lastTime);

        SKSE::log::info("D3DRenderer: ImGui context initialised");
        return true;
    }

    static void RenderFrame()
    {
        if (!g_renderCallback)
            return;

        if (!g_initialized)
        {
            g_initialized = Init();
            if (!g_initialized)
                return;
        }

        auto *rd = RE::BSGraphics::Renderer::GetRendererData();
        if (!rd)
            return;

        auto *swapChain = reinterpret_cast<IDXGISwapChain *>(rd->renderWindows[0].swapChain);
        if (!swapChain)
            return;

        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        g_deltaTime = static_cast<float>(now.QuadPart - g_lastTime.QuadPart) /
                      static_cast<float>(g_freq.QuadPart);
        g_lastTime = now;
        if (g_deltaTime <= 0.0f || g_deltaTime > 1.0f)
            g_deltaTime = 0.016f;

        DXGI_SWAP_CHAIN_DESC desc{};
        if (SUCCEEDED(swapChain->GetDesc(&desc)))
        {
            g_displayW = static_cast<float>(desc.BufferDesc.Width);
            g_displayH = static_cast<float>(desc.BufferDesc.Height);
        }

        ImGui::SetCurrentContext(g_imguiContext);
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2(g_displayW, g_displayH);
        io.DeltaTime = g_deltaTime;

        ImGui_ImplDX11_NewFrame();
        ImGui::NewFrame();
        g_renderCallback();
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    struct RenderUIHook
    {
        static void thunk()
        {
            func();
            RenderFrame();
        }
        static inline REL::Relocation<decltype(thunk) *> func;
    };

    void Install()
    {
        static bool s_installed = false;
        if (s_installed)
            return;
        s_installed = true;

        RenderUIHook::func = SKSE::GetTrampoline().write_call<5>(
            REL::RelocationID(35556, 36555).address() +
                (REL::Module::IsAE() ? 0x371 : 0x3AB),
            RenderUIHook::thunk);

        SKSE::log::info("D3DRenderer: hooked RenderUIHook");
    }

    void SetRenderCallback(void (*callback)()) { g_renderCallback = callback; }

    float GetDeltaTime() { return g_deltaTime; }
    float GetDisplayWidth() { return g_displayW; }
    float GetDisplayHeight() { return g_displayH; }

    void AddRectFilled(float x0, float y0, float x1, float y1, unsigned int color)
    {
        if (!g_imguiContext)
            return;
        ImDrawList *dl = ImGui::GetForegroundDrawList();
        dl->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), color);
    }

    void AddText(float x, float y, float size, unsigned int color, ImFont *font, const char *text)
    {
        if (!g_imguiContext || !text || !*text)
            return;
        if (!font || !font->IsLoaded())
        {
            SKSE::log::warn("AddText: font not ready");
            return;
        }
        ImDrawList *dl = ImGui::GetForegroundDrawList();
        dl->AddText(font, size, ImVec2(x, y), color, text);
    }

    ImFont *TextFont() { return font_text; }
    ImFont *IconFont() { return font_icon; }
}
