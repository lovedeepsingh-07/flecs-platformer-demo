#include "scene.hpp"

void Scene::MainMenuScene::on_enter(GameContext::GameContext& ctx) {}
void Scene::MainMenuScene::on_exit(GameContext::GameContext& ctx) {
    ctx.registry.reset();
}
void Scene::MainMenuScene::on_render(GameContext::GameContext& ctx) {}
void Scene::MainMenuScene::on_update(GameContext::GameContext& ctx) {}
