#include "components.hpp"
#include "systems.hpp"
#include <box2d/box2d.h>

void HealthSystem::setup(GameContext::GameContext& ctx) {
    ctx.registry
        .observer<components::AttackHitEventComponent, components::HealthComponent>()
        .event(flecs::OnSet)
        .each([](flecs::entity curr_entity, const components::AttackHitEventComponent& attack_hit_event,
                 components::HealthComponent& health) {
            if (!(health.health <= 0)) {
                health.health -= 5;
            } else {
                health.health = health.max_health;
            }
            curr_entity.remove<components::AttackHitEventComponent>();
        });
}
