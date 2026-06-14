#pragma once

#include "Widget.h"

namespace InfoWidgets
{
    class GeneralConfig
    {
    public:
        std::string widgetConfigName() { return "General"; }
        void configure(const toml::table &root);
        bool renderConfig(toml::table &root);
        void saveConfig(toml::table &root);

        bool isHudVisible() const { return _hudVisible.load(); }
        RE::BSTEventSink<RE::InputEvent *> *inputSink() { return &_sink; }

    private:
        std::atomic<bool>     _hudVisible{true};
        std::atomic<uint32_t> _toggleKey{0}; // stores ImGuiMCP::ImGuiKey
        std::atomic<bool>     _capturingKey{false};

        class Sink : public RE::BSTEventSink<RE::InputEvent *>
        {
        public:
            Sink(GeneralConfig &owner) : _owner(owner) {}
            RE::BSEventNotifyControl ProcessEvent(
                RE::InputEvent *const *events,
                RE::BSTEventSource<RE::InputEvent *> *) override;

        private:
            GeneralConfig &_owner;
        } _sink{*this};
    };
}
