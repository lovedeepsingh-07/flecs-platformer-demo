#pragma once

#include "event_engine.hpp"
#include "flecs.h"

struct GameContext {
    flecs::world registry;
    EventEngine::EventEngine event_system;
};
