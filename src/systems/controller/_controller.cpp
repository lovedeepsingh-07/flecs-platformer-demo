#include "systems.hpp"

void systems::controller::setup(flecs::world& registry) {
    registry
        .system<components::Controller, components::Movement>("Handle "
                                                              "Controller Input")
        .kind(flecs::PreUpdate)
        .run([](flecs::iter& iter) {
            // NOTE: ofcourse, we can easily split the
            // handle_horizontal_movement,handle_jump,handle_dash etc into
            // different systems, but the main issue with that approach is this
            // `curr_key_pressed` thing, and at the time of me writing this
            // comment, I have no idea when I might need this again and there
            // can only be one of these so I just converted all the systems into
            // functions and just added to the same system
            auto curr_key_pressed = (KeyboardKey)GetKeyPressed();

            while (iter.next()) {
                auto controller = iter.field<components::Controller>(0);
                auto movement = iter.field<components::Movement>(1);

                for (auto i : iter) {
                    flecs::entity curr_entity = iter.entity(i);
                    handle_horizontal_movement(curr_entity, movement[i], controller[i]);
                    handle_jump(curr_entity, movement[i], controller[i]);
                    handle_dash(curr_entity, movement[i], controller[i], curr_key_pressed);
                    handle_attack(curr_entity, movement[i], controller[i]);
                }
            }
        })
        .add<components::system_types::Update>();
};
