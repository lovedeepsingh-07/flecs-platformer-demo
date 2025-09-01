#include "systems.hpp"

void systems::freeze_frame(flecs::world& registry) {
    registry.system("Freeze Frame System")
        .kind(flecs::PreUpdate)
        .run([](flecs::iter& iter) {
            flecs::world registry = iter.world();
            if (registry.has<components::global_options::Freezed>()) {
                auto& freezed = registry.get_mut<components::global_options::Freezed>();
                freezed.freeze_time -= registry.delta_time();
                if (freezed.freeze_time <= 0) {
                    registry.remove<components::global_options::Freezed>();
                }
            }
        })
        .add<components::system_types::Update_FreezeFrame>();
}
