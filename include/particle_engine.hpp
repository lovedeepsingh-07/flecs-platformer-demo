#pragma once

#include "error.hpp"
#include <functional>
#include <random>
#include <raylib.h>
#include <raymath.h>
#include <sol/sol.hpp>
#include <tl/expected.hpp>

namespace ParticleEngine {
    struct Particle {
        Vector2 pos;
        Vector2 vel;
        bool alive{ false };
        float lifetime;
        float age{ 0.0F };
    };

    struct EmitterConfig {
        bool local_coords{ false };
        bool one_shot{ false };
        int amount{ 20 };
        float speed_scale{ 1.0F };
        float lifetime{ 1.0F };
        float velocity_scale{ 90.0F };
        float explosiveness{ 0.0F };
        float direction_bias{ 0 };
        float spread{ PI * 2.0F };
        float separation{ 0.0F };
        bool square_particles{ true };
        float start_size{ 7.0F }, end_size{ 3.0F };
        Vector4 start_color{ 1.0F, 1.0F, 1.0F, 1.0F }, end_color{ 1.0F, 1.0F, 1.0F, 0.0F };
    };

    class ParticleEngine {
      private:
        std::unordered_map<std::string, EmitterConfig> m_emitter_configs;

      public:
        ParticleEngine() = default;
        ~ParticleEngine() = default;

        tl::expected<void, error::ParticleEngineError> setup();
        const EmitterConfig& get_config(const std::string& config_id) const;
    };

    class ParticleEmitter {
      private:
        std::vector<Particle> particle_pool;
        float cycle_time{ 0.0F };
        int next_slot{ 0 };
        std::mt19937 rand_gen;

        void reset_cycle();
        void spawn_n(Vector2 spawn_pos, int count);

      public:
        // engine configuration
        int pool_size;
        bool emitting{ true };
        EmitterConfig config;
        Vector2 emitter_pos;

        // drawing functions
        std::function<void(Vector2 pos, float radius, Vector4 color)> render_circle_fcn{ nullptr };
        std::function<void(Vector2 pos, float size, Vector4 color)> render_square_fcn{ nullptr };

        ParticleEmitter() = default;
        ~ParticleEmitter() = default;

        void setup();
        Particle* alloc_particle();
        void emit(Vector2 emit_pos, float delta_time);
        void update(float delta_time);
        void render();
        void kill_particles();
    };
}
