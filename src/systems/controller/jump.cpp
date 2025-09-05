#include "constants.hpp"
#include "systems.hpp"

void systems::controller::handle_jump(
    flecs::entity curr_entity, components::Movement& movement, components::Controller& controller
) {
    flecs::world registry = curr_entity.world();
    const auto& controller_engine = registry.get<components::Controller_Engine>();
    const auto& curr_keymap = controller_engine.engine.get_keymap(controller._id);

    if (IsKeyPressed(curr_keymap.jump)
        && !curr_entity.has<components::events::BufferedJumpEvent>()
        && !curr_entity.has<components::events::HurtEvent>()) {
        if (movement.on_ground) {
            // if we are on the ground then we just add <JumpEvent,JumpEvent_One>
            flecs::entity jump_entity =
                registry.entity()
                    .add<components::events::JumpEvent, components::events::JumpEvent_One>()
                    .child_of(curr_entity);
            curr_entity.add<components::Jump_Entity>(jump_entity);
        } else {
            flecs::entity jump_entity = curr_entity.target<components::Jump_Entity>();
            if (jump_entity.is_valid()) {
                // if we have already jumped once, then we just add <JumpEvent,JumpEvent_Two>
                if (jump_entity.has<components::events::JumpEvent, components::events::JumpEvent_One>()) {
                    jump_entity.add<components::events::JumpEvent, components::events::JumpEvent_Two>();
                }
                // if we have already jumped twice, then we just buffer the jump
                if (jump_entity.has<components::events::JumpEvent, components::events::JumpEvent_Two>()) {
                    curr_entity.set<components::events::BufferedJumpEvent>(
                        { 20 * constants::FRAMES_TO_SEC }
                    );
                }
            } else {
                // if we are in the air and we have jumped yet, then we just add <JumpEvent,JumpEvent_Two>
                flecs::entity jump_entity =
                    registry.entity()
                        .add<components::events::JumpEvent, components::events::JumpEvent_Two>()
                        .child_of(curr_entity);
                curr_entity.add<components::Jump_Entity>(jump_entity);
            }
        }
    }
}
