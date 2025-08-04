#include "components.hpp"
#include "systems.hpp"

void AttackSystem::update(GameContext::GameContext& ctx) {
    ctx.registry
        .system<components::AttackComponent, components::AnimationComponent>()
        .each([](flecs::entity curr_entity, components::AttackComponent& attack,
                 components::AnimationComponent& animation) {
            if (curr_entity.has<components::AttackEventComponent>() && !attack.attacking) {
                attack.attacking = true;
            }
        })
        .run();
}
