#include "PCH.h"
#include "PluginHUD.h"
#include "Version.h"

namespace
{
    void SetupLog()
    {
        const auto logsFolder = SKSE::log::log_directory();
        if (!logsFolder)
        {
            SKSE::stl::report_and_fail("SKSE log directory unavailable");
        }

        const auto pluginLogPath = *logsFolder / (std::string(Plugin::Name) + ".log");

        auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
            pluginLogPath.string(), true);
        auto msvcSink = std::make_shared<spdlog::sinks::msvc_sink_mt>();

        auto logger = std::make_shared<spdlog::logger>(
            Plugin::Name,
            spdlog::sinks_init_list{fileSink, msvcSink});

        logger->set_level(spdlog::level::trace);
        logger->flush_on(spdlog::level::trace);
        spdlog::set_default_logger(std::move(logger));
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%s:%#] %v");
    }

    void OnSKSEMessage(SKSE::MessagingInterface::Message *msg)
    {
        if (!msg)
            return;

        switch (msg->type)
        {
        case SKSE::MessagingInterface::kDataLoaded:
            InfoWidgets::PluginHUD::OnDataLoaded();
            break;
        default:
            break;
        }
    }
}

SKSEPluginInfo(
        .Version = Plugin::Version,
        .Name = Plugin::Name,
        .Author = "Elsky");

SKSEPluginLoad(const SKSE::LoadInterface *skse)
{
    SKSE::Init(skse);

    // 14 is the minimum for one write_call<5>, 64 gives margin for a second hook
    SKSE::AllocTrampoline(64);

    SetupLog();

    InfoWidgets::PluginHUD::LoadConfig();

    SKSE::log::info("{} v{} loading", Plugin::Name, Plugin::Version.string("."));
    SKSE::log::info("Game version: {}", skse->RuntimeVersion().string("."));

    if (const auto *msg = SKSE::GetMessagingInterface())
    {
        msg->RegisterListener(OnSKSEMessage);
    }
    else
    {
        SKSE::log::error("SKSE messaging interface unavailable");
        return false;
    }

    SKSE::log::info("{} loaded successfully", Plugin::Name);
    return true;
}
