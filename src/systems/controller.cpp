#include "components.hpp"
#include "systems.hpp"

void systems::controller(flecs::world& registry) {
    registry
        .system<components::Controller, components::Movement>("Handle "
                                                              "Controller Input")
        .kind(flecs::PreUpdate)
        .each([](const components::Controller& controller, components::Movement& movement) {
            if (controller._id == 0) {
                if (IsKeyDown(KEY_A)) {
                    movement.left_idle_right = -1;
                } else if (IsKeyDown(KEY_D)) {
                    movement.left_idle_right = 1;
                } else {
                    movement.left_idle_right = 0;
                }
            }
            if (controller._id == 1) {
                if (IsKeyDown(KEY_H)) {
                    movement.left_idle_right = -1;
                } else if (IsKeyDown(KEY_L)) {
                    movement.left_idle_right = 1;
                } else {
                    movement.left_idle_right = 0;
                }
            }
        });
};
