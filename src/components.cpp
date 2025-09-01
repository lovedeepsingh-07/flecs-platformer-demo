#include "components.hpp"

void components::setup(flecs::world& registry) {
    registry.component<components::global_options::Paused>().add(flecs::Singleton);
    registry.component<components::global_options::Freezed>().add(flecs::Singleton);
    registry.component<components::global_options::DebugMode>().add(flecs::Singleton);
    registry.component<components::global_options::GameFonts>().add(flecs::Singleton);

    registry.component<components::system_types::Global>();
    registry.component<components::system_types::Update>();
    registry.component<components::system_types::Update_FreezeFrame>();
    registry.component<components::system_types::Render>();

    registry.component<components::SceneRoot>();
    registry.component<components::ActiveScene>().add(flecs::Relationship).add(flecs::Exclusive);
    registry.component<components::MainMenu_Scene>().add(flecs::Target);
    registry.component<components::Game_Scene>().add(flecs::Target);

    registry.component<components::Texture_Engine>().add(flecs::Singleton);
    registry.component<components::State_Engine>().add(flecs::Singleton);
    registry.component<components::Controller_Engine>().add(flecs::Singleton);

    registry.component<components::emitter_types::JumpEmitter>()
        .add(flecs::Relationship)
        .add(flecs::Exclusive);
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

    registry.component<components::Jump_Entity>().add(flecs::Relationship);
    registry.component<components::Dash_Entity>().add(flecs::Relationship);
    registry.component<components::Hitbox_Entity>().add(flecs::Relationship);

    registry.component<components::events::GameQuitEvent>().add(flecs::Singleton);
    registry.component<components::events::JumpEvent>().add(flecs::Relationship).add(flecs::Exclusive);
    registry.component<components::events::JumpEvent_One>().add(flecs::Target);
    registry.component<components::events::JumpEvent_Two>().add(flecs::Target);
    registry.component<components::events::BufferedDashEvent>();
    registry.component<components::events::DashEvent>();
    registry.component<components::events::AttackEvent>();
    registry.component<components::events::HitEvent>();
}
