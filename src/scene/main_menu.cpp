#include "scene.hpp"
#include "systems.hpp"

void MainMenuScene::on_enter(flecs::world& registry) {}
void MainMenuScene::on_exit(flecs::world& registry) {
    registry.reset();
}
void MainMenuScene::on_render(flecs::world& registry) {
    Render2DSystem::update(registry);
}
void MainMenuScene::on_update(flecs::world& registry) {}
