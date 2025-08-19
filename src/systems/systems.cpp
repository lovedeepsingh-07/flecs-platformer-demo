#include "systems.hpp"

void systems::setup(flecs::world& registry) {
    registry.system("Quit Events").kind(flecs::PostLoad).run([](flecs::iter& iter) {
        flecs::world registry = iter.world();
        if (WindowShouldClose()) {
            registry.quit();
        }
    });

    systems::controller(registry);
    systems::render(registry);
};
