#pragma once

#include "PCH.h"

namespace InfoWidgets::FlatRegenCache
{
    struct Values
    {
        float health{0.0f};
        float magicka{0.0f};
        float stamina{0.0f};
    };

    void tick(float deltaTime);

    const Values &get();
}
