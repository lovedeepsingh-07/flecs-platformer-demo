#pragma once

#include "event_system.hpp"
#include "flecs.h"

struct GameContext {
    flecs::world registry;
    EventSystem::EventSystem event_system;
    bool should_quit_game;
};
