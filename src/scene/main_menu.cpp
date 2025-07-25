#include "scene.hpp"
#include "systems.hpp"

void MainMenuScene::on_enter(GameContext& ctx) {}
void MainMenuScene::on_exit(GameContext& ctx) {
    ctx.registry.reset();
}
void MainMenuScene::on_render(GameContext& ctx) {
    Render2DSystem::update(ctx);
}
void MainMenuScene::on_update(GameContext& ctx) {}
