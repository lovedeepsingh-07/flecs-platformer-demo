#include "constants.hpp"
#include "systems.hpp"

void systems::controller::handle_dash(
    flecs::entity curr_entity, components::Movement& movement, components::Controller& controller, int curr_key_pressed
) {
    flecs::world registry = curr_entity.world();
    const auto& controller_engine = registry.get<components::Controller_Engine>();
    const auto& curr_keymap = controller_engine.engine.get_keymap(controller._id);

    bool is_blocking = curr_entity.target<components::Block_Entity>().is_valid();

    if ((IsKeyPressed(curr_keymap.left) || IsKeyPressed(curr_keymap.right))
        && !curr_entity.target<components::Dash_Entity>().is_valid() && !is_blocking) {
        if (curr_entity.has<components::events::BufferedDashEvent>()) {
            if (curr_key_pressed
                == curr_entity.get<components::events::BufferedDashEvent>().buffer_key) {
                // if we have already buffered the the dash, and it
                // was in the same direction as the current keypress, then we add <DashEvent> and remove <BufferedDashEvent>
                flecs::entity dash_entity =
                    registry.entity()
                        .set<components::events::DashEvent>({ .dash_time = 20 * constants::FRAMES_TO_SEC })
                        .child_of(curr_entity);
                curr_entity.add<components::Dash_Entity>(dash_entity);
                curr_entity.remove<components::events::BufferedDashEvent>();
            } else {
                // if we have already buffered the the dash, and it
                // was in the opposite direction as the current keypress,
                // then we remove previous <BufferedDashEvent> and add a new <BufferedDashEvent> in this direction
                curr_entity.remove<components::events::BufferedDashEvent>();
                curr_entity.set<components::events::BufferedDashEvent>(
                    { .buffer_key = (KeyboardKey)curr_key_pressed,
                      .buffer_time = 20 * constants::FRAMES_TO_SEC }
                );
            }
        } else {
            // if we have not buffered the dash, then we just buffer it
            curr_entity.set<components::events::BufferedDashEvent>(
                { .buffer_key = (KeyboardKey)curr_key_pressed,
                  .buffer_time = 20 * constants::FRAMES_TO_SEC }
            );
        }
    }
}
