#include "PluginHUD.h"

#include <cerrno>
#include <cstring>
#include <filesystem>

#include "PCH.h"
#include "AttackSpeedWidget.h"
#include "D3DRenderer.h"
#include "FlatRegenCache.h"
#include "FoodWidget.h"
#include "GeneralConfig.h"
#include "LightWidget.h"
#include "HealthRegenWidget.h"
#include "MagickaRegenWidget.h"
#include "MovementSpeedWidget.h"
#include "NoiseWidget.h"
#include "ResistWidget.h"
#include "StaminaRegenWidget.h"
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
    static MagickaRegenTextWidget magickaRegenTextWidget;
    WIDGET_RENDER_FUNCTION(magickaRegenTextWidget);
    static MagickaRegenIconWidget magickaRegenIconWidget;
    WIDGET_RENDER_FUNCTION(magickaRegenIconWidget);
    static HealthRegenTextWidget healthRegenTextWidget;
    WIDGET_RENDER_FUNCTION(healthRegenTextWidget);
    static HealthRegenIconWidget healthRegenIconWidget;
    WIDGET_RENDER_FUNCTION(healthRegenIconWidget);
    static StaminaRegenTextWidget staminaRegenTextWidget;
    WIDGET_RENDER_FUNCTION(staminaRegenTextWidget);
    static StaminaRegenIconWidget staminaRegenIconWidget;
    WIDGET_RENDER_FUNCTION(staminaRegenIconWidget);
    static MagicResistTextWidget magicResistTextWidget;
    WIDGET_RENDER_FUNCTION(magicResistTextWidget);
    static MagicResistIconWidget magicResistIconWidget;
    WIDGET_RENDER_FUNCTION(magicResistIconWidget);
    static FireResistTextWidget fireResistTextWidget;
    WIDGET_RENDER_FUNCTION(fireResistTextWidget);
    static FireResistIconWidget fireResistIconWidget;
    WIDGET_RENDER_FUNCTION(fireResistIconWidget);
    static FrostResistTextWidget frostResistTextWidget;
    WIDGET_RENDER_FUNCTION(frostResistTextWidget);
    static FrostResistIconWidget frostResistIconWidget;
    WIDGET_RENDER_FUNCTION(frostResistIconWidget);
    static ShockResistTextWidget shockResistTextWidget;
    WIDGET_RENDER_FUNCTION(shockResistTextWidget);
    static ShockResistIconWidget shockResistIconWidget;
    WIDGET_RENDER_FUNCTION(shockResistIconWidget);
    static PoisonResistTextWidget poisonResistTextWidget;
    WIDGET_RENDER_FUNCTION(poisonResistTextWidget);
    static PoisonResistIconWidget poisonResistIconWidget;
    WIDGET_RENDER_FUNCTION(poisonResistIconWidget);
    static DiseaseResistTextWidget diseaseResistTextWidget;
    WIDGET_RENDER_FUNCTION(diseaseResistTextWidget);
    static DiseaseResistIconWidget diseaseResistIconWidget;
    WIDGET_RENDER_FUNCTION(diseaseResistIconWidget);
    static ArmorRatingTextWidget armorRatingTextWidget;
    WIDGET_RENDER_FUNCTION(armorRatingTextWidget);
    static ArmorRatingIconWidget armorRatingIconWidget;
    WIDGET_RENDER_FUNCTION(armorRatingIconWidget);
    static MovementSpeedTextWidget movementSpeedTextWidget;
    WIDGET_RENDER_FUNCTION(movementSpeedTextWidget);
    static MovementSpeedIconWidget movementSpeedIconWidget;
    WIDGET_RENDER_FUNCTION(movementSpeedIconWidget);

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
        &toxicityIconWidget,
        &magickaRegenTextWidget,
        &magickaRegenIconWidget,
        &healthRegenTextWidget,
        &healthRegenIconWidget,
        &staminaRegenTextWidget,
        &staminaRegenIconWidget,
        &magicResistTextWidget,
        &magicResistIconWidget,
        &fireResistTextWidget,
        &fireResistIconWidget,
        &frostResistTextWidget,
        &frostResistIconWidget,
        &shockResistTextWidget,
        &shockResistIconWidget,
        &poisonResistTextWidget,
        &poisonResistIconWidget,
        &diseaseResistTextWidget,
        &diseaseResistIconWidget,
        &armorRatingTextWidget,
        &armorRatingIconWidget,
        &movementSpeedTextWidget,
        &movementSpeedIconWidget};

    static void saveConfig()
    {
        generalConfig.saveConfig(config);
        for (auto *widget : widgets)
        {
            widget->saveConfig(config);
        }

        try
        {
            std::filesystem::path p(configFile);
            if (p.has_parent_path())
            {
                std::filesystem::create_directories(p.parent_path());
            }
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            SKSE::log::error("Failed to create parent directories for config file {}: {}", configFile, e.what());
            return;
        }

        std::ofstream file(configFile);
        if (file.is_open())
        {
            SKSE::log::info("Wrote file {}", configFile);
            file << config;
        }
        else
        {
            SKSE::log::error("Failed to open config file {} for writing: {}", configFile, std::strerror(errno));
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

        if (!ui->IsShowingMenus())
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
        FlatRegenCache::tick(deltaTime);
        for (auto *widget : widgets)
        {
            widget->updateAndRender(deltaTime);
        }
    }

    void LoadConfig()
    {
        if (!std::filesystem::exists(configFile))
        {
            SKSE::log::info("No config file {} found", configFile);
            std::ofstream{configFile}.close();
            return;
        }
        try
        {
            SKSE::log::info("Found config file {}, parsing it now", configFile);
            config = toml::parse_file(configFile);
        }
        catch (const toml::parse_error &e)
        {
            SKSE::log::error("LoadConfig: failed to parse '{}': {}", configFile, e.description());
            return;
        }
        SKSE::log::info("Parse done, now changing the configuration");
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
        SKSEMenuFramework::AddSectionItem("Widgets / Magicka Regen Text", rendermagickaRegenTextWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Magicka Regen Icon", rendermagickaRegenIconWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Health Regen Text", renderhealthRegenTextWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Health Regen Icon", renderhealthRegenIconWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Stamina Regen Text", renderstaminaRegenTextWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Stamina Regen Icon", renderstaminaRegenIconWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Magic Resist Text", rendermagicResistTextWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Magic Resist Icon", rendermagicResistIconWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Fire Resist Text", renderfireResistTextWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Fire Resist Icon", renderfireResistIconWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Frost Resist Text", renderfrostResistTextWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Frost Resist Icon", renderfrostResistIconWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Shock Resist Text", rendershockResistTextWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Shock Resist Icon", rendershockResistIconWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Poison Resist Text", renderpoisonResistTextWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Poison Resist Icon", renderpoisonResistIconWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Disease Resist Text", renderdiseaseResistTextWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Disease Resist Icon", renderdiseaseResistIconWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Armor Rating Text", renderarmorRatingTextWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Armor Rating Icon", renderarmorRatingIconWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Movement Speed Text", rendermovementSpeedTextWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Widgets / Movement Speed Icon", rendermovementSpeedIconWidgetSettings);
        SKSEMenuFramework::AddSectionItem("Save and Load Configuration", renderSaveLoadConfiguration);
        SKSEMenuFramework::AddSectionItem("About", renderAbout);
    }

}
