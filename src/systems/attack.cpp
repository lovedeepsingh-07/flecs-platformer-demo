#include "components.hpp"
#include "systems.hpp"

void AttackSystem::update(GameContext::GameContext& ctx) {
    flecs::system attack_sys =
        ctx.registry
            .system<components::AttackComponent, components::AnimationComponent>()
            .each([](flecs::entity curr_entity, components::AttackComponent& attack,
                     components::AnimationComponent& animation) {

            });
    attack_sys.run();
}
