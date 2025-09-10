#include "particle_engine.hpp"

tl::expected<void, error::ParticleEngineError> ParticleEngine::ParticleEngine::setup() {
    sol::state lua_state;
    lua_state.open_libraries(sol::lib::base);
    lua_state.script_file("data/emitters.lua");

    sol::object EMITTERS_object = lua_state["EMITTERS"];
    if (!EMITTERS_object.valid() || EMITTERS_object.get_type() != sol::type::table) {
        return tl::unexpected(error::ParticleEngineError{
            error::ParticleEngineError::Type::LoadError, "unable to read 'EMITTERS' table" });
    }
    sol::table EMITTERS_table = EMITTERS_object.as<sol::table>();

    for (auto&& emitter_kv : EMITTERS_table) {
        sol::object emitter_config_object = emitter_kv.second;
        if (!emitter_config_object.valid()
            || emitter_config_object.get_type() != sol::type::table) {
            return tl::unexpected(error::ParticleEngineError{
                error::ParticleEngineError::Type::LoadError,
                "unable to read emitter_config table" });
        }
        sol::table emitter_config_table = emitter_config_object.as<sol::table>();

        std::string emitter_config_id = emitter_kv.first.as<std::string>();
        EmitterConfig curr_config{
            .local_coords = emitter_config_table["local_coords"],
            .one_shot = emitter_config_table["one_shot"],
            .amount = emitter_config_table["amount"],
            .speed_scale = emitter_config_table["speed_scale"],
            .lifetime = emitter_config_table["lifetime"],
            .velocity_scale = emitter_config_table["velocity_scale"],
            .explosiveness = emitter_config_table["explosiveness"],
            .direction_bias = emitter_config_table["direction_bias"],
            .spread = emitter_config_table["spread"],
            .separation = emitter_config_table["separation"],
            .square_particles = emitter_config_table["square_particles"],
            .start_size = emitter_config_table["start_size"],
            .end_size = emitter_config_table["end_size"],
            .start_color = {
				emitter_config_table["start_color"][1],
				emitter_config_table["start_color"][2],
				emitter_config_table["start_color"][3],
				emitter_config_table["start_color"][4],
			},
			.end_color = {
				emitter_config_table["end_color"][1],
				emitter_config_table["end_color"][2],
				emitter_config_table["end_color"][3],
				emitter_config_table["end_color"][4],
			},
        };
        curr_config.direction_bias = curr_config.direction_bias * DEG2RAD;
        curr_config.spread = curr_config.spread * DEG2RAD;
        curr_config.separation = curr_config.separation * DEG2RAD;
        this->m_emitter_configs.emplace(emitter_config_id, curr_config);
    }

    return {};
}
const ParticleEngine::EmitterConfig&
ParticleEngine::ParticleEngine::get_config(const std::string& config_id) const {
    return this->m_emitter_configs.at(config_id);
}

void ParticleEngine::ParticleEmitter::setup() {
    // setup random
    this->rand_gen = std::mt19937(std::random_device{}());

    // resize particle_pool
    this->particle_pool.resize(this->pool_size);

    // add rendering functions
    this->render_circle_fcn = std::move([](Vector2 pos, float radius, Vector4 color) {
        auto render_color =
            Color{ (std::uint8_t)(color.x * 255), (std::uint8_t)(color.y * 255),
                   (std::uint8_t)(color.z * 255), (std::uint8_t)(color.w * 255) };
        DrawCircle((int)pos.x, (int)pos.y, radius, render_color);
    });
    this->render_square_fcn = std::move([](Vector2 pos, float size, Vector4 color) {
        auto render_color =
            Color{ (std::uint8_t)(color.x * 255), (std::uint8_t)(color.y * 255),
                   (std::uint8_t)(color.z * 255), (std::uint8_t)(color.w * 255) };
        DrawRectangle((int)(pos.x - (size / 2)), (int)(pos.y - (size / 2)), (int)size, (int)size, render_color);
    });
}

void ParticleEngine::ParticleEmitter::reset_cycle() {
    cycle_time = 0.0F;
    next_slot = 0;
}

ParticleEngine::Particle* ParticleEngine::ParticleEmitter::alloc_particle() {
    for (auto& curr_particle : this->particle_pool) {
        if (!curr_particle.alive) {
            curr_particle.alive = true;
            curr_particle.age = 0.0F;
            return &curr_particle;
        }
    }
    return nullptr;
}

void ParticleEngine::ParticleEmitter::spawn_n(Vector2 spawn_pos, int count) {
    for (int i = 0; i < count; i++) {
        // clamp the number of particles spawned
        if (i > (int)this->particle_pool.size()) {
            break;
        }
        Particle* curr_particle = this->alloc_particle();
        if (curr_particle == nullptr) {
            return;
        }

        // generate a random angle
        bool right_lobe = (rand_gen() % 2 == 0);
        float mean = this->config.direction_bias
            + (right_lobe ? +this->config.separation * 0.5F : -this->config.separation * 0.5F);
        std::normal_distribution<float> random_angle_gen(mean, this->config.spread);
        float random_angle = random_angle_gen(this->rand_gen);

        // generate a random velocity
        // std::uniform_real_distribution<float> random_vel_gen(0.6F, 1.0F);
        // float random_vel = random_vel_gen(this->rand_gen);

        curr_particle->age = 0.0F;
        curr_particle->lifetime = this->config.lifetime;

        // the particle's position will be relative to the emitter
        if (this->config.local_coords) {
            curr_particle->pos = Vector2{ 0.0F, 0.0F };
        } else {
            // the particle's position will be according to world_coords
            curr_particle->pos = spawn_pos;
        }

        curr_particle->vel =
            Vector2{ std::cos(random_angle) * (this->config.velocity_scale),
                     std::sin(random_angle) * (this->config.velocity_scale) };
    }
}

void ParticleEngine::ParticleEmitter::emit(Vector2 emit_pos, float delta_time) {
    if (!emitting) {
        return;
    }

    this->emitter_pos = emit_pos;

    float dt = delta_time * this->config.speed_scale;

    // emit all particles at once
    if (this->config.explosiveness >= 1.0F) {
        if (this->next_slot == 0) {
            this->spawn_n(emit_pos, this->config.amount);
            this->next_slot = this->config.amount; // this is important to prevent spawning more than the "amount" specified
        }
        cycle_time += dt;
        if (cycle_time >= this->config.lifetime) {
            cycle_time -= this->config.lifetime;
            this->reset_cycle();
        }
        return;
    }

    // emit a continuous stream of particles
    this->cycle_time += dt;
    const float spawn_window = this->config.lifetime * (1 - this->config.explosiveness);
    while (this->next_slot < this->config.amount) {
        float spawn_at_time =
            ((float)this->next_slot / (float)(this->config.amount - 1)) * spawn_window;
        if (spawn_at_time <= this->cycle_time) {
            this->spawn_n(emit_pos, 1);
            this->next_slot += 1;
        } else {
            break;
        }
    }
    if (this->cycle_time >= this->config.lifetime) {
        if (this->config.one_shot) {
            this->emitting = false;
        }
        this->reset_cycle();
    }
}

void ParticleEngine::ParticleEmitter::update(float delta_time) {
    for (auto& curr_particle : this->particle_pool) {
        if (!curr_particle.alive) {
            continue;
        }
        curr_particle.age += delta_time;
        if (curr_particle.age >= curr_particle.lifetime) {
            curr_particle.alive = false;
            continue;
        }

        curr_particle.pos =
            Vector2Add(curr_particle.pos, Vector2Scale(curr_particle.vel, delta_time));
    }
};

void ParticleEngine::ParticleEmitter::render() {
    for (auto& curr_particle : this->particle_pool) {
        if (!curr_particle.alive) {
            continue;
        }

        float life_ratio = curr_particle.age / curr_particle.lifetime;
        float render_size = Lerp(this->config.start_size, this->config.end_size, life_ratio);
        Vector4 render_color =
            Vector4Lerp(this->config.start_color, this->config.end_color, life_ratio);

        Vector2 draw_pos;
        if (this->config.local_coords) {
            draw_pos = Vector2Add(curr_particle.pos, this->emitter_pos);
        } else {
            draw_pos = curr_particle.pos;
        }

        if (this->config.square_particles) {
            this->render_square_fcn(draw_pos, render_size * 2, render_color);
        } else {
            this->render_circle_fcn(draw_pos, render_size, render_color);
        }
    }
}

void ParticleEngine::ParticleEmitter::kill_particles() {
    for (auto& curr_particle : this->particle_pool) {
        if (curr_particle.alive) {
            curr_particle.alive = false;
        }
    }
}
