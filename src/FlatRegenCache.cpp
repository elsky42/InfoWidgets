#include "FlatRegenCache.h"

namespace InfoWidgets::FlatRegenCache
{
    static Values _values;
    static float _accumulator{1.0f}; // start at 1 to force scan on first tick

    void tick(float deltaTime)
    {
        _accumulator += deltaTime;
        if (_accumulator < 1.0f)
            return;
        _accumulator = 0.0f;

        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player)
            return;
        auto *effectList = player->AsMagicTarget()->GetActiveEffectList();
        if (!effectList)
            return;

        _values = {};
        for (auto *effect : *effectList)
        {
            if (!effect)
                continue;
            if (effect->flags.any(RE::ActiveEffect::Flag::kInactive) ||
                effect->flags.any(RE::ActiveEffect::Flag::kDispelled))
                continue;
            // must have remaining duration (excludes instant one-shot restores)
            if (effect->duration <= 0.0f || effect->elapsedSeconds >= effect->duration)
                continue;
            if (effect->magnitude <= 0.0f)
                continue;

            auto *base = effect->GetBaseObject();
            if (!base)
                continue;

            // kRecover indicates "fortify" effects (the AV recovers to its old value when
            // the effect ends, i.e. it raises the base/max rather than restoring current).
            // We want "restore" effects — those have kRecover unset.
            if (base->data.flags.any(RE::EffectSetting::EffectSettingData::Flag::kRecover))
                continue;

            auto accumulate = [&](RE::ActorValue av) {
                switch (av)
                {
                case RE::ActorValue::kHealth:  _values.health  += effect->magnitude; break;
                case RE::ActorValue::kMagicka: _values.magicka += effect->magnitude; break;
                case RE::ActorValue::kStamina: _values.stamina += effect->magnitude; break;
                default: break;
                }
            };

            accumulate(base->data.primaryAV);
            const auto archetype = base->GetArchetype();
            if (archetype == RE::EffectArchetypes::ArchetypeID::kDualValueModifier)
                accumulate(base->data.secondaryAV);
        }
    }

    const Values& get() { return _values; }
}
