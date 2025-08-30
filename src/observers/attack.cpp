#include "components.hpp"
#include "constants.hpp"
#include "observers.hpp"

void observers::attack(flecs::world& registry) {
    registry.observer<components::events::AttackEvent>("AttackEvent on_remove")
        .event(flecs::OnRemove)
        .each([](flecs::entity curr_entity, const components::events::AttackEvent& attack_event) {
            flecs::world registry = curr_entity.world();
            flecs::entity hitbox_entity = curr_entity.target<components::Hitbox>();

            if (hitbox_entity.is_valid()) {
                hitbox_entity.destruct();
            }
        });
    registry
        .observer<components::events::HitEvent, components::Health, components::PhysicalBody, components::TextureComponent>(
            "HitEvent "
            "on_set"
        )
        .event(flecs::OnSet)
        .each([](flecs::entity curr_entity, const components::events::HitEvent& hit_event,
                 components::Health& health, const components::PhysicalBody& body,
                 components::TextureComponent& texture) {
            // apply damage
            if (!(health.health <= 0)) {
                health.health -= 5;
            } else {
                health.health = health.max_health;
            }
            // apply knockback
            b2Vec2 vel = b2Body_GetLinearVelocity(body.body_id);
            vel.x = 2 * constants::MAX_PLAYER_RUN_VEL * (float)hit_event.direction;
            b2Body_SetLinearVelocity(body.body_id, vel);

            texture.flipped = (hit_event.direction != -1);

            if (curr_entity.has<components::events::AttackEvent>()) {
                curr_entity.remove<components::events::AttackEvent>();
            }
        });
};
