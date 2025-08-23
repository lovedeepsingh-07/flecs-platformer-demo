#include "components.hpp"
#include "observers.hpp"

void observers::attack(flecs::world& registry) {
    registry.observer<components::AttackEvent>("AttackEvent on_remove")
        .event(flecs::OnRemove)
        .each([](flecs::entity curr_entity, components::AttackEvent& attack_event) {
            flecs::world registry = curr_entity.world();
            flecs::entity hitbox_entity = curr_entity.lookup("hitbox");

            if (hitbox_entity.is_valid()) {
                hitbox_entity.destruct();
            }
        });
};
