#include "components.hpp"
#include "systems.hpp"
#include <box2d/box2d.h>

void KnockbackSystem::setup(GameContext::GameContext& ctx) {
    ctx.registry.observer<components::AttackHitEventComponent>()
        .event(flecs::OnAdd)
        .each([](flecs::entity curr_entity,
                 const components::AttackHitEventComponent& attack_hit_event) {
            // TODO: in order for this to work, it requires a refactor of the movement system
            // b2Vec2 vel = b2Body_GetLinearVelocity(phy.body_id);
            // if (!(health.health <= 0)) {
            //     health.health -= 5;
            // } else {
            //     health.health = health.max_health;
            // }
            // curr_entity.remove<components::AttackHitEventComponent>();
        });
}
