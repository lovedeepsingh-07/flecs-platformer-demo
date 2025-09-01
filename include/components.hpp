#pragma once

#include "controller_engine.hpp"
#include "particle_engine.hpp"
#include "state_engine.hpp"
#include "texture_engine.hpp"
#include <array>
#include <box2d/box2d.h>
#include <flecs.h>
#include <raylib.h>

namespace components {
    namespace global_options {
        struct Paused {};
        struct Freezed {
            float freeze_time;
        };
        struct DebugMode {};
        struct GameFonts {
            std::array<Font, 1> font_list;
        };
    }

    namespace system_types {
        struct Global {};
        struct Update {};
        struct Update_FreezeFrame {}; // this is only for FreezeFrame system
        struct Render {};
    };

    struct SceneRoot {};
    // this represents an exclusive relationship (there can be only 1 active scene)
    struct ActiveScene {};
    // these below are the "targets" of that relationship
    struct MainMenu_Scene {};
    struct Game_Scene {};

    struct Texture_Engine {
        TextureEngine::TextureEngine engine;
    };
    struct State_Engine {
        StateEngine::StateEngine engine;
    };
    struct Controller_Engine {
        ControllerEngine::ControllerEngine engine;
    };

    struct Particle_Emitter {
        ParticleEngine::ParticleEngine engine;
    };
    // these are relationships and the "target" of these relationships will be the emitter
    namespace emitter_types {
        struct JumpEmitter {};
    }

    struct GlobalCamera {
        Camera2D camera;
    };
    struct Camera_Target {};


    struct Controller {
        uint8_t _id;
    };
    struct Position {
        float x;
        float y;
    };
    struct Movement {
        int left_idle_right{ 0 };
        bool on_ground;
    };
    struct Health {
        float health;
        float max_health;
    };

    struct RectangleComponent {
        float width;
        float height;
        Color color;
    };
    // RectOpts -> settings for rectangles
    // Lines -> this is the lines version of a rectangle
    // Debug -> this version of a rectangle is supposed to be rendered only during debug mode
    namespace rectangle_options {
        struct RectOpts_Lines {};
        struct RectOpts_Debug {};
    }

    struct TextureComponent {
        Texture2D texture;
        Rectangle source_rect;
        bool flipped;
    };
    struct State {
        std::string curr_state_id;
        std::string state_registry_id;
    };
    struct Animation {
        int curr_frame_index = 0;
        float time_accumulator = 0.0F;
        bool playing = true;
    };

    struct PhysicalWorld {
        b2WorldId world_id;
    };
    struct PhysicalDebugDraw {
        b2DebugDraw debug_draw;
    };
    struct PhysicalBody {
        b2BodyId body_id;
    };
    struct CastQueryFilter {
        b2QueryFilter filter;
    };
    struct BaseCollider {
        float width;
        float height;
    };

    // the "target" of this relationship will be the jump_entity itself
    struct Attack_Entity {};
    struct Jump_Entity {};
    struct Dash_Entity {};
    struct Hitbox_Entity {};

    namespace events {
        struct GameQuitEvent {};
        struct JumpEvent {};
        struct JumpEvent_One {};
        struct JumpEvent_Two {};
        struct BufferedJumpEvent {
            float buffer_time;
        };
        struct BufferedDashEvent {
            KeyboardKey buffer_key;
            float buffer_time;
        };
        struct DashEvent {
            float dash_time;
        };
        struct AttackEvent {
            bool hit_some_entity;
        };
        struct HitEvent {
            int direction;
        };
    }

    void setup(flecs::world& registry);
}
