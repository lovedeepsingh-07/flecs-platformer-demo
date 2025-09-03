#pragma once

#include "components.hpp"
#include <flecs.h>

namespace scene {
    namespace main_menu {
        void on_enter(flecs::iter& iter, std::size_t, components::ActiveScene);
        void on_exit(flecs::iter& iter, std::size_t, components::ActiveScene);
    };
    namespace setting {
        void on_enter(flecs::iter& iter, std::size_t, components::ActiveScene);
        void on_exit(flecs::iter& iter, std::size_t, components::ActiveScene);
    };
    namespace game_setup {
        void on_enter(flecs::iter& iter, std::size_t, components::ActiveScene);
        void on_exit(flecs::iter& iter, std::size_t, components::ActiveScene);
    };
    namespace game {
        void setup_player(flecs::world& registry, b2WorldId world_id, b2Vec2 pos);
        void setup_enemy(flecs::world& registry, b2WorldId world_id, b2Vec2 pos);
        void setup_tile_world(flecs::world& registry, b2WorldId world_id);

        void on_enter(flecs::iter& iter, std::size_t, components::ActiveScene);
        void on_exit(flecs::iter& iter, std::size_t, components::ActiveScene);
    };
};
