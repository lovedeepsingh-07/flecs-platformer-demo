#include "systems.hpp"

void systems::setup(flecs::world& registry) {
    systems::global_events(registry);
    systems::controller::setup(registry);
    systems::movement(registry);
    systems::physics(registry);
    systems::state(registry);
    systems::block(registry);
    systems::attack(registry);
    systems::freeze_frame(registry);
    systems::camera(registry);
    systems::animation(registry);
    systems::particles(registry);
    systems::render::setup(registry);
};
