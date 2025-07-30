#include "components.hpp"
#include "systems.hpp"

void AttackSystem::update(GameContext& ctx) {
    flecs::query<> attack_query =
        ctx.registry.query_builder().with<components::AttackComponent>().build();
    ctx.registry.defer_begin();
    attack_query.run([](flecs::iter& iter) {
        while (iter.next()) {
            auto attack = iter.field<components::AttackComponent>(0);
            for (auto i : iter) {
                flecs::entity curr_entity = iter.entity(i);
                if (curr_entity.has<components::AttackEventComponent>()
                    && !attack[i].attacking) {
                    attack[i].attacking = true;
                    curr_entity.remove<components::JumpEventComponent>();
                }
            }
        }
    });
    ctx.registry.defer_end();
}
