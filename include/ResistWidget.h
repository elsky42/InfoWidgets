#pragma once

#include "BaseTextWidget.h"

namespace InfoWidgets
{

    class MagicResistTextWidget final : public TextWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };

    class MagicResistIconWidget final : public IconWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };

    class FireResistTextWidget final : public TextWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };

    class FireResistIconWidget final : public IconWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };

    class FrostResistTextWidget final : public TextWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };

    class FrostResistIconWidget final : public IconWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };

    class ShockResistTextWidget final : public TextWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };

    class ShockResistIconWidget final : public IconWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };

    class PoisonResistTextWidget final : public TextWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };

    class PoisonResistIconWidget final : public IconWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };

    class DiseaseResistTextWidget final : public TextWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };

    class DiseaseResistIconWidget final : public IconWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };

    class ArmorRatingTextWidget final : public TextWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };

    class ArmorRatingIconWidget final : public IconWidget
    {
    public:
        std::string widgetConfigName() override;
        void update() override;
    };
}
