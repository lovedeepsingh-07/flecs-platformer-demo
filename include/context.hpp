#pragma once

#include "event_engine.hpp"
#include "flecs.h"
#include "texture_engine.hpp"

struct GameContext {
    flecs::world registry;
    EventEngine::EventEngine event_system;
    TextureEngine texture_engine;
};
