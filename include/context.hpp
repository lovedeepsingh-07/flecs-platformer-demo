#pragma once

#include "event_engine.hpp"
#include "state_engine.hpp"
#include "texture_engine.hpp"
#include <flecs.h>

struct GameContext {
    flecs::world registry;
    EventEngine::EventEngine event_system;
    TextureEngine texture_engine;
    StateEngine::StateEngine state_engine;
};
