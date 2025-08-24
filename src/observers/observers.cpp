#include "observers.hpp"

void observers::setup(flecs::world& registry) {
    observers::scene_manager(registry);
    observers::movement(registry);
    observers::attack(registry);
};
