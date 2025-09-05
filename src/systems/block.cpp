#include "systems.hpp"

void systems::block(flecs::world& registry) {
    registry.system<components::events::ParryWindowEvent>("Parry Window System")
        .kind(flecs::PreUpdate)
        .each([](flecs::entity curr_entity, components::events::ParryWindowEvent& parry_window_event) {
            parry_window_event.parry_time -= curr_entity.world().delta_time();
            if (parry_window_event.parry_time <= 0) {
                curr_entity.remove<components::events::ParryWindowEvent>();
            }
        })
        .add<components::system_types::Update>();
}
