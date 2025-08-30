#include "components.hpp"

void components::setup(flecs::world& registry) {
    registry.component<components::global_options::Paused>().add(flecs::Singleton);
    registry.component<components::global_options::Freezed>().add(flecs::Singleton);
    registry.component<components::global_options::DebugMode>().add(flecs::Singleton);
    registry.component<components::global_options::GameFonts>().add(flecs::Singleton);

    registry.component<components::SceneRoot>();
    registry.component<components::ActiveScene>().add(flecs::Relationship).add(flecs::Exclusive);
    registry.component<components::MainMenu_Scene>();
    registry.component<components::Game_Scene>();

    registry.component<components::Texture_Engine>().add(flecs::Singleton);
    registry.component<components::State_Engine>().add(flecs::Singleton);

    registry.component<components::emitter_types::JumpEmitter>().add(flecs::Exclusive);
    registry.component<components::Particle_Emitter>();


    registry.component<components::GlobalCamera>();
    registry.component<components::Camera_Target>();
    registry.component<components::Controller>();

    registry.component<components::Position>();

    registry.component<components::RectangleComponent>();
    registry.component<components::rectangle_options::RectOpts_Lines>();
    registry.component<components::rectangle_options::RectOpts_Debug>();

    registry.component<components::TextureComponent>();
    registry.component<components::State>();
    registry.component<components::Animation>();

    registry.component<components::PhysicalWorld>().add(flecs::Singleton);
    registry.component<components::PhysicalDebugDraw>().add(flecs::Singleton);
    registry.component<components::PhysicalBody>();
    registry.component<components::CastQueryFilter>();
    registry.component<components::BaseCollider>();

    registry.component<components::Movement>();
    registry.component<components::Health>();

    registry.component<components::events::JumpEvent>();
    registry.component<components::events::AttackEvent>();
    registry.component<components::events::HitEvent>();

    registry.component<components::Hitbox>();
}
