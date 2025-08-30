#include "observers.hpp"
#include "components.hpp"

void observers::setup(flecs::world& registry) {
    registry.observer<components::global_options::Paused>("Paused on_add")
        .event(flecs::OnAdd)
        .run([](flecs::iter& iter) {
            flecs::world registry = iter.world();
            registry.each<components::system_types::Update>(
                [](flecs::entity curr_system, const components::system_types::Update&) {
                    curr_system.disable();
                }
            );
            registry.each<components::system_types::Update_FreezeFrame>(
                [](flecs::entity curr_system, const components::system_types::Update_FreezeFrame&) {
                    curr_system.disable();
                }
            );
        });
    registry.observer<components::global_options::Paused>("Paused on_remove")
        .event(flecs::OnRemove)
        .run([](flecs::iter& iter) {
            flecs::world registry = iter.world();

            registry.each<components::system_types::Update>(
                [](flecs::entity curr_system, const components::system_types::Update&) {
                    curr_system.enable();
                }
            );
            registry.each<components::system_types::Update_FreezeFrame>(
                [](flecs::entity curr_system, const components::system_types::Update_FreezeFrame&) {
                    curr_system.enable();
                }
            );
        });

    registry.observer<components::global_options::Freezed>("Freezed on_add")
        .event(flecs::OnAdd)
        .run([](flecs::iter& iter) {
            flecs::world registry = iter.world();
            registry.each<components::system_types::Update>(
                [](flecs::entity curr_system, const components::system_types::Update&) {
                    curr_system.disable();
                }
            );
        });
    registry.observer<components::global_options::Freezed>("Freezed on_remove")
        .event(flecs::OnRemove)
        .run([](flecs::iter& iter) {
            flecs::world registry = iter.world();
            registry.each<components::system_types::Update>(
                [](flecs::entity curr_system, const components::system_types::Update&) {
                    curr_system.enable();
                }
            );
        });

    observers::scene_manager(registry);
    observers::movement(registry);
    observers::attack(registry);
};
