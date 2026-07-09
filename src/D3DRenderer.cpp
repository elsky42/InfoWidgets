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

    static std::string g_textFontPath;
    static float g_textFontSize = 20.0f;
    static bool g_fontReloadPending = false;

    static std::vector<FontEntry> g_availableFonts;
    static bool g_fontsScanned = false;

    static void ScanFontsDir(const std::filesystem::path &dir, std::vector<FontEntry> &out)
    {
        std::error_code ec;
        for (auto it = std::filesystem::recursive_directory_iterator(dir, ec);
             it != std::filesystem::recursive_directory_iterator();
             it.increment(ec))
        {
            if (ec)
                break;
            if (!it->is_regular_file(ec) || ec)
                continue;
            const auto &p = it->path();
            auto ext = p.extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return std::tolower(c); });
            if (ext != ".ttf" && ext != ".otf")
                continue;
            if (p.filename() == "fa-solid-900.ttf")
                continue;
            out.push_back({p.stem().string(), p.string()});
        }
    }

    static void TryLoadTextFont(ImGuiIO &io)
    {
        ImFontConfig cfg{};
        cfg.PixelSnapH = true;
        cfg.OversampleH = 1;
        cfg.OversampleV = 1;

        auto tryPath = [&](const char *path) -> bool {
            if (!path || !*path || !std::filesystem::exists(path))
                return false;
            font_text = io.Fonts->AddFontFromFileTTF(path, g_textFontSize, &cfg, nullptr);
            if (font_text)
                SKSE::log::info("D3DRenderer: loaded text font '{}'", path);
            else
                SKSE::log::error("D3DRenderer: failed to load font '{}'", path);
            return font_text != nullptr;
        };

        if (!g_textFontPath.empty() && tryPath(g_textFontPath.c_str()))
            return;
        tryPath("Data/SKSE/Plugins/InfoWidgets/Fonts/static/Jost-Regular.ttf");
    }

    static void TryLoadFontAwesome(ImGuiIO &io)
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

        font_icon = io.Fonts->AddFontFromFileTTF(path, g_textFontSize, &cfg, faRanges);
        if (font_icon)
            SKSE::log::info("D3DRenderer: loaded Font Awesome");
        else
            SKSE::log::warn("D3DRenderer: Font Awesome not found, icon widgets will be invisible");
    }

    static void ReloadFonts()
    {
        ImGui::SetCurrentContext(g_imguiContext);
        ImGuiIO &io = ImGui::GetIO();
        font_text = nullptr;
        font_icon = nullptr;
        ImGui_ImplDX11_InvalidateDeviceObjects();
        io.Fonts->Clear();
        TryLoadTextFont(io);
        TryLoadFontAwesome(io);
        if (!font_text)
            font_text = io.Fonts->AddFontDefault();
        io.Fonts->Build();
        ImGui_ImplDX11_CreateDeviceObjects();
        SKSE::log::info("D3DRenderer: font atlas rebuilt");
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

        TryLoadTextFont(io);
        TryLoadFontAwesome(io);
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

        if (g_fontReloadPending)
        {
            g_fontReloadPending = false;
            ReloadFonts();
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

    const std::vector<FontEntry>& AvailableFonts()
    {
        if (g_fontsScanned)
            return g_availableFonts;
        g_fontsScanned = true;

        ScanFontsDir("Data/SKSE/Plugins/InfoWidgets/Fonts", g_availableFonts);

        char windir[MAX_PATH]{};
        if (GetEnvironmentVariableA("WINDIR", windir, MAX_PATH) > 0)
            ScanFontsDir(std::filesystem::path(windir) / "Fonts", g_availableFonts);

        std::sort(g_availableFonts.begin(), g_availableFonts.end(),
                  [](const FontEntry &a, const FontEntry &b) { return a.displayName < b.displayName; });

        return g_availableFonts;
    }

    void SetTextFontPath(std::string path)
    {
        g_textFontPath = std::move(path);
        if (g_initialized) g_fontReloadPending = true;
    }

    void SetTextFontSize(float size)
    {
        g_textFontSize = size;
        if (g_initialized) g_fontReloadPending = true;
    }
}
