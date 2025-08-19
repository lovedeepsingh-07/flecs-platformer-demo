#include "components.hpp"
#include "systems.hpp"

void systems::controller(flecs::world& registry) {
    registry
        .system<components::Controller, components::Position>("Handle "
                                                              "Controller Input")
        .kind(flecs::PreUpdate)
        .each([](const components::Controller& controller, components::Position& pos) {
            if (controller._id == 0) {
                if (IsKeyDown(KEY_A)) {
                    pos.x -= 5;
                } else if (IsKeyDown(KEY_D)) {
                    pos.x += 5;
                }
                if (IsKeyDown(KEY_W)) {
                    pos.y -= 5;
                } else if (IsKeyDown(KEY_S)) {
                    pos.y += 5;
                }
            }
            if (controller._id == 1) {
                if (IsKeyDown(KEY_H)) {
                    pos.x -= 5;
                } else if (IsKeyDown(KEY_L)) {
                    pos.x += 5;
                }
                if (IsKeyDown(KEY_K)) {
                    pos.y -= 5;
                } else if (IsKeyDown(KEY_J)) {
                    pos.y += 5;
                }
            }
        });
};
