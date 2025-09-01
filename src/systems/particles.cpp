#include "systems.hpp"

void systems::particles(flecs::world& registry) {
    registry
        .system<components::Particle_Emitter, components::Position>(
            "Handle Particle Emission"
        )
        .kind(flecs::PreUpdate)
        .each([](flecs::entity curr_entity, components::Particle_Emitter& particle_emitter,
                 const components::Position& pos) {
            flecs::world registry = curr_entity.world();

            particle_emitter.engine.emit(Vector2{ pos.x, pos.y }, registry.delta_time());
            particle_emitter.engine.update(registry.delta_time());
        })
        .add<components::system_types::Update>();
}
