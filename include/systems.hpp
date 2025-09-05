#pragma once

#include "components.hpp"
#include <clay_raylib.hpp>
#include <flecs.h>

namespace systems {
    void setup(flecs::world& registry);

    void global_events(flecs::world& registry);
    namespace controller {
        void setup(flecs::world& registry);
        void handle_horizontal_movement(
            flecs::entity curr_entity, components::Movement& movement, components::Controller& controller
        );
        void handle_jump(flecs::entity curr_entity, components::Movement& movement, components::Controller& controller);
        void handle_dash(
            flecs::entity curr_entity,
            components::Movement& movement,
            components::Controller& controller,
            int curr_key_pressed
        );
        void handle_attack(flecs::entity curr_entity, components::Movement& movement, components::Controller& controller);
    }
    void movement(flecs::world& registry);
    void physics(flecs::world& registry);
    void state(flecs::world& registry);
    void block(flecs::world& registry);
    void attack(flecs::world& registry);
    void freeze_frame(flecs::world& registry);
    void camera(flecs::world& registry);
    void animation(flecs::world& registry);
    void particles(flecs::world& registry);

    namespace render {
        void setup(flecs::world& registry);

        void texture(flecs::world& registry);
        void rectangle(flecs::world& registry);
        void physics(flecs::world& registry);
        void GUI(flecs::world& registry);
    }
}
