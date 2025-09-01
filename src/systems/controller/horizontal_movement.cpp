#include "systems.hpp"

void systems::controller::handle_horizontal_movement(
    flecs::entity curr_entity, components::Movement& movement, components::Controller& controller
) {
    flecs::world registry = curr_entity.world();
    const auto& controller_engine = registry.get<components::Controller_Engine>();
    const auto& curr_keymap = controller_engine.engine.get_keymap(controller._id);

    if (IsKeyDown(curr_keymap.left) && !curr_entity.has<components::events::AttackEvent>()) {
        movement.left_idle_right = -1;
    } else if (IsKeyDown(curr_keymap.right)
               && !curr_entity.has<components::events::AttackEvent>()) {
        movement.left_idle_right = 1;
    } else {
        movement.left_idle_right = 0;
    }
}
