#include "components.hpp"

void components::setup(flecs::world& registry) {
    registry.component<Paused>().add(flecs::Singleton);
    registry.component<Freezed>().add(flecs::Singleton);

    registry.component<SceneRoot>();
    registry.component<ActiveScene>().add(flecs::Exclusive);
    registry.component<MainMenu_Scene>();
    registry.component<Game_Scene>();

    registry.component<GameFonts>().add(flecs::Singleton);

    registry.component<Controller>();

    registry.component<Position>();

    registry.component<Rectangle>();
}
