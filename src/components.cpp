#include "components.hpp"

void components::setup(flecs::world& registry) {
    registry.component<components::Paused>().add(flecs::Singleton);
    registry.component<components::Freezed>().add(flecs::Singleton);
    registry.component<components::DebugMode>().add(flecs::Singleton);
    registry.component<components::PhysicalDebugDraw>().add(flecs::Singleton);

    registry.component<components::SceneRoot>();
    registry.component<components::ActiveScene>().add(flecs::Exclusive);
    registry.component<components::MainMenu_Scene>();
    registry.component<components::Game_Scene>();

    registry.component<components::Texture_Engine>().add(flecs::Singleton);
    registry.component<components::State_Engine>().add(flecs::Singleton);
    registry.component<components::Particle_Emitter>();

    registry.component<components::GameFonts>().add(flecs::Singleton);

    registry.component<components::GlobalCamera>();
    registry.component<components::Camera_Target>();
    registry.component<components::Controller>();

    registry.component<components::Position>();

    registry.component<components::RectangleComponent>();
    registry.component<components::RectOpts_Lines>();
    registry.component<components::RectOpts_Debug>();

    registry.component<components::TextureComponent>();
    registry.component<components::State>();
    registry.component<components::Animation>();

    registry.component<components::PhysicalWorld>().add(flecs::Singleton);
    registry.component<components::PhysicalBody>();
    registry.component<components::CastQueryFilter>();
    registry.component<components::BaseCollider>();

    registry.component<components::Movement>();
    registry.component<components::Health>();

    registry.component<components::JumpEvent>();
    registry.component<components::AttackEvent>();
    registry.component<components::HitEvent>();
}
