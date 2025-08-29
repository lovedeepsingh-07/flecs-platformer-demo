#include "particle_engine.hpp"

void ParticleEngine::ParticleEngine::setup() {
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

void ParticleEngine::ParticleEngine::reset_cycle() {
    cycle_time = 0.0F;
    next_slot = 0;
}

ParticleEngine::Particle* ParticleEngine::ParticleEngine::alloc_particle() {
    for (auto& curr_particle : this->particle_pool) {
        if (!curr_particle.alive) {
            curr_particle.alive = true;
            curr_particle.age = 0.0F;
            return &curr_particle;
        }
    }
    return nullptr;
}

void ParticleEngine::ParticleEngine::spawn_n(Vector2 spawn_pos, int count) {
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

void ParticleEngine::ParticleEngine::emit(Vector2 emit_pos, float delta_time) {
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

void ParticleEngine::ParticleEngine::update(float delta_time) {
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

void ParticleEngine::ParticleEngine::render() {
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

void ParticleEngine::ParticleEngine::kill_particles() {
    for (auto& curr_particle : this->particle_pool) {
        if (curr_particle.alive) {
            curr_particle.alive = false;
        }
    }
}
