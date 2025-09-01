#include "systems.hpp"

void systems::controller::handle_attack(
    flecs::entity curr_entity, components::Movement& movement, components::Controller& controller
) {
    flecs::world registry = curr_entity.world();
    const auto& controller_engine = registry.get<components::Controller_Engine>();
    const auto& curr_keymap = controller_engine.engine.get_keymap(controller._id);

    if (IsKeyPressed(curr_keymap.attack)
        && !curr_entity.target<components::Attack_Entity>().is_valid()
        && !curr_entity.has<components::events::HitEvent>()) {
        flecs::entity attack_entity =
            registry.entity()
                .set<components::events::AttackEvent>({ .hit_some_entity = false })
                .child_of(curr_entity);
        curr_entity.add<components::Attack_Entity>(attack_entity);

        // when we press the attack button, if a jump has been buffered, then that buffered jump will be cancelled
        if (curr_entity.has<components::events::BufferedJumpEvent>()) {
            curr_entity.remove<components::events::BufferedJumpEvent>();
        }
    }
}
