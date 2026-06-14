#include "PluginHUD.h"

#include "PCH.h"
#include "AttackSpeedWidget.h"
#include "D3DRenderer.h"
#include "FoodWidget.h"
#include "GeneralConfig.h"
#include "LightWidget.h"
#include "NoiseWidget.h"
#include "ToxicityWidget.h"
#include "VampireStageWidget.h"
#include "Version.h"

// bypass the limitations of AddSectionItem
#define WIDGET_RENDER_FUNCTION(widget)     \
    static void render##widget##Settings() \
    {                                      \
        widget.renderConfig(config);       \
    }

namespace InfoWidgets::PluginHUD
{

    constexpr auto configFile = "Data/SKSE/Plugins/InfoWidgets.toml";
    static toml::table config;

    static GeneralConfig generalConfig;
    static void renderGeneralConfig() { generalConfig.renderConfig(config); }

    static NoiseTextWidget noiseTextWidget;
    WIDGET_RENDER_FUNCTION(noiseTextWidget);
    static NoiseIconWidget noiseIconWidget;
    WIDGET_RENDER_FUNCTION(noiseIconWidget);
    static LightTextWidget lightTextWidget;
    WIDGET_RENDER_FUNCTION(lightTextWidget);
    static LightIconWidget lightIconWidget;
    WIDGET_RENDER_FUNCTION(lightIconWidget);
    static MainCourseTextWidget mainCourseTextWidget;
    WIDGET_RENDER_FUNCTION(mainCourseTextWidget);
    static SnackTextWidget snackTextWidget;
    WIDGET_RENDER_FUNCTION(snackTextWidget);
    static DrinkTextWidget drinkTextWidget;
    WIDGET_RENDER_FUNCTION(drinkTextWidget);
    static FoodIconWidget foodIconWidget;
    WIDGET_RENDER_FUNCTION(foodIconWidget);
    static VampireStageTextWidget vampireStageTextWidget;
    WIDGET_RENDER_FUNCTION(vampireStageTextWidget);
    static VampireStageIconWidget vampireStageIconWidget;
    WIDGET_RENDER_FUNCTION(vampireStageIconWidget);
    static MainHandAttackSpeedTextWidget mainHandAttackSpeedTextWidget;
    WIDGET_RENDER_FUNCTION(mainHandAttackSpeedTextWidget);
    static OffHandAttackSpeedTextWidget offHandAttackSpeedTextWidget;
    WIDGET_RENDER_FUNCTION(offHandAttackSpeedTextWidget);
    static MainHandAttackSpeedIconWidget mainHandAttackSpeedIconWidget;
    WIDGET_RENDER_FUNCTION(mainHandAttackSpeedIconWidget);
    static OffHandAttackSpeedIconWidget offHandAttackSpeedIconWidget;
    WIDGET_RENDER_FUNCTION(offHandAttackSpeedIconWidget);
    static ToxicityTextWidget toxicityTextWidget;
    WIDGET_RENDER_FUNCTION(toxicityTextWidget);
    static ToxicityIconWidget toxicityIconWidget;
    WIDGET_RENDER_FUNCTION(toxicityIconWidget);

    static std::vector<Widget *> widgets = {
        &noiseTextWidget,
        &noiseIconWidget,
        &lightTextWidget,
        &lightIconWidget,
        &mainCourseTextWidget,
        &snackTextWidget,
        &drinkTextWidget,
        &foodIconWidget,
        &vampireStageTextWidget,
        &vampireStageIconWidget,
        &mainHandAttackSpeedTextWidget,
        &offHandAttackSpeedTextWidget,
        &mainHandAttackSpeedIconWidget,
        &offHandAttackSpeedIconWidget,
        &toxicityTextWidget,
        &toxicityIconWidget};

    static void saveConfig()
    {
        generalConfig.saveConfig(config);
        for (auto *widget : widgets)
        {
            widget->saveConfig(config);
        }
        std::ofstream file(configFile);
        if (file.is_open())
        {
            file << config;
        }
        else
        {
            SKSE::log::error("Failed to open config file for writing");
        }
    }

    static void renderSaveLoadConfiguration()
    {
        ImGuiMCP::ImVec2 regionAvail;
        ImGuiMCP::ImGui::GetContentRegionAvail(&regionAvail);
        float buttonWidth = regionAvail.x * 0.5f - ImGuiMCP::ImGui::GetStyle()->ItemSpacing.x;
        if (ImGuiMCP::ImGui::Button("Save Configuration to File", ImGuiMCP::ImVec2(buttonWidth, 0)))
        {
            saveConfig();
        }
        ImGuiMCP::ImGui::SameLine();
        if (ImGuiMCP::ImGui::Button("Load Configuration from File", ImGuiMCP::ImVec2(buttonWidth, 0)))
        {
            ImGuiMCP::ImGui::OpenPopup("Confirm Load?");
        }

        auto viewport = ImGuiMCP::ImGui::GetMainViewport();
        ImGuiMCP::ImVec2 center = {
            viewport->Pos.x + viewport->Size.x * 0.5f,
            viewport->Pos.y + viewport->Size.y * 0.5f};
        ImGuiMCP::ImGui::SetNextWindowPos(center, ImGuiMCP::ImGuiCond_Appearing, ImGuiMCP::ImVec2(0.5f, 0.5f));

        if (ImGuiMCP::ImGui::BeginPopupModal("Confirm Load?", nullptr, ImGuiMCP::ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGuiMCP::ImGui::Text("This will overwrite your current settings.\nAre you sure?\n\n");
            ImGuiMCP::ImGui::Separator();

            if (ImGuiMCP::ImGui::Button("OK", ImGuiMCP::ImVec2(120, 0)))
            {
                LoadConfig();
                ImGuiMCP::ImGui::CloseCurrentPopup();
            }

            ImGuiMCP::ImGui::SetItemDefaultFocus();
            ImGuiMCP::ImGui::SameLine();

            if (ImGuiMCP::ImGui::Button("Cancel", ImGuiMCP::ImVec2(120, 0)))
            {
                ImGuiMCP::ImGui::CloseCurrentPopup();
            }

            ImGuiMCP::ImGui::EndPopup();
        }
    }

    static void renderAbout()
    {
        ImGuiMCP::ImGui::Text("Version: %s", Plugin::Version.string(".").c_str());
    }

    static bool ShouldDisplay()
    {
        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player || !player->GetParentCell())
            return false;

        if (player->IsDead())
            return false;

        auto *ui = RE::UI::GetSingleton();
        if (!ui)
            return false;

        if (!ui->IsMenuOpen(RE::HUDMenu::MENU_NAME))
            return false;

        if (SKSEMenuFramework::IsAnyBlockingWindowOpened())
            return true;

        return !ui->GameIsPaused() &&
               !ui->IsItemMenuOpen() &&
               !ui->IsApplicationMenuOpen() &&
               !ui->IsModalMenuOpen() &&
               !ui->IsMenuOpen(RE::MapMenu::MENU_NAME) &&
               !ui->IsMenuOpen(RE::LoadWaitSpinner::MENU_NAME) &&
               !ui->IsMenuOpen(RE::TweenMenu::MENU_NAME) &&
               !ui->IsMenuOpen(RE::Console::MENU_NAME) &&
               !ui->IsMenuOpen("BSGamepadCamera") &&
               !ui->IsMenuOpen("SystemMenu") &&
               !ui->IsMenuOpen(RE::JournalMenu::MENU_NAME) &&
               !ui->IsMenuOpen(RE::LoadingMenu::MENU_NAME) &&
               !ui->IsMenuOpen(RE::FaderMenu::MENU_NAME) &&
               !ui->IsMenuOpen(RE::DialogueMenu::MENU_NAME);
    }

    static void RenderHUD()
    {
        if (!generalConfig.isHudVisible())
            return;

        if (!ShouldDisplay())
            return;

        const float deltaTime = D3DRenderer::GetDeltaTime();
        for (auto *widget : widgets)
        {
            widget->updateAndRender(deltaTime);
        }
    }

    void LoadConfig()
    {
        if (!std::filesystem::exists(configFile))
        {
            std::ofstream{configFile}.close();
            return;
        }
        try
        {
            config = toml::parse_file(configFile);
        }
        catch (const toml::parse_error &e)
        {
            SKSE::log::error("LoadConfig: failed to parse '{}': {}", configFile, e.description());
            return;
        }
        generalConfig.configure(config);
        for (auto *widget : widgets)
        {
            widget->configure(config);
        }
    }

    void OnDataLoaded()
    {
        SKSE::log::info("SKSEMenuFramework installed: {}", SKSEMenuFramework::IsInstalled());

        D3DRenderer::SetRenderCallback(RenderHUD);
        D3DRenderer::Install();

        RE::BSInputDeviceManager::GetSingleton()->AddEventSink(generalConfig.inputSink());

        SKSEMenuFramework::SetSection(Plugin::Name);
        SKSEMenuFramework::AddSectionItem("General", renderGeneralConfig);
        SKSEMenuFramework::AddSectionItem("Widgets / Noise Text", rendernoiseTextWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Noise Icon", rendernoiseIconWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Light Text", renderlightTextWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Light Icon", renderlightIconWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Main Course Text", rendermainCourseTextWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Snack Text", rendersnackTextWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Drink Text", renderdrinkTextWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Food Icon", renderfoodIconWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Vampire Stage Text", rendervampireStageTextWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Vampire Stage Icon", rendervampireStageIconWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Main Hand Attack Speed Text", rendermainHandAttackSpeedTextWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Main Hand Attack Speed Icon", rendermainHandAttackSpeedIconWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Off Hand Attack Speed Text", renderoffHandAttackSpeedTextWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Off Hand Attack Speed Icon", renderoffHandAttackSpeedIconWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Toxicity Text", rendertoxicityTextWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Toxicity Icon", rendertoxicityIconWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Save and Load Configuration", renderSaveLoadConfiguration);
        SKSEMenuFramework::AddSectionItem("About", renderAbout);
    }

}
