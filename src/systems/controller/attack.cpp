#include "constants.hpp"
#include "systems.hpp"

void systems::controller::handle_attack(
    flecs::entity curr_entity, components::Movement& movement, components::Controller& controller
) {
    flecs::world registry = curr_entity.world();
    const auto& controller_engine = registry.get<components::Controller_Engine>();
    const auto& curr_keymap = controller_engine.engine.get_keymap(controller._id);

    if ((IsKeyPressed(curr_keymap.light_attack) || IsKeyPressed(curr_keymap.heavy_attack))
        && !curr_entity.target<components::Attack_Entity>().is_valid()
        && !curr_entity.has<components::events::HurtEvent>()) {
        flecs::entity attack_entity = registry.entity().child_of(curr_entity);
        if (IsKeyDown(curr_keymap.light_attack)) {
            if (IsKeyDown(curr_keymap.down)) {
                attack_entity.add<components::attack_types::Light, components::attack_types::direction::Down>();
            } else {
                attack_entity.add<components::attack_types::Light, components::attack_types::direction::Forward>();
            }
        }
        if (IsKeyDown(curr_keymap.heavy_attack)) {
            if (IsKeyDown(curr_keymap.down)) {
                attack_entity.add<components::attack_types::Heavy, components::attack_types::direction::Down>();
            } else {
                attack_entity.add<components::attack_types::Heavy, components::attack_types::direction::Forward>();
            }
        }
        attack_entity.set<components::events::AttackEvent>({ .hit_some_entity = false });
        curr_entity.add<components::Attack_Entity>(attack_entity);

        // when we press the attack button, if a jump has been buffered, then that buffered jump will be cancelled
        if (curr_entity.has<components::events::BufferedJumpEvent>()) {
            curr_entity.remove<components::events::BufferedJumpEvent>();
        }
    }

    bool is_blocking = curr_entity.target<components::Block_Entity>().is_valid();
    if (IsKeyPressed(curr_keymap.block) && !is_blocking
        && !curr_entity.has<components::events::ParryWindowEvent>()
        && !curr_entity.has<components::events::HurtEvent>()) {
        curr_entity.set<components::events::ParryWindowEvent>(
            { .parry_time = 20 * constants::FRAMES_TO_SEC }
        );
        flecs::entity block_entity = registry.entity().child_of(curr_entity);
        curr_entity.add<components::Block_Entity>(block_entity);
    }
    if (IsKeyReleased(curr_keymap.block) && is_blocking) {
        if (curr_entity.has<components::events::ParryWindowEvent>()) {
            curr_entity.remove<components::events::ParryWindowEvent>();
        }
        curr_entity.target<components::Block_Entity>().destruct();
    }
}
