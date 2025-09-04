#include "texture_engine.hpp"

void TextureEngine::TextureEngine::setup() {
    auto cowboy_render_scale = Vector2{ 1.0F, 1.0F };
    this->load_texture("cowboy_idle", "assets/cowboy/idle.png", cowboy_render_scale);
    this->load_texture("cowboy_run", "assets/cowboy/run.png", cowboy_render_scale);
    this->load_texture("cowboy_jump", "assets/cowboy/jump.png", cowboy_render_scale);
    this->load_texture("cowboy_attack", "assets/cowboy/sword_slash_vertical.png", cowboy_render_scale);
    this->load_texture("cowboy_attack_air", "assets/cowboy/horizontal_air_slash.png", cowboy_render_scale);
    this->load_texture("cowboy_hurt", "assets/cowboy/hurt.png", cowboy_render_scale);
    this->load_texture("cowboy_dash", "assets/cowboy/dash.png", cowboy_render_scale);

    auto decidueye_render_scale = Vector2{ 0.55F, 0.4F };
    this->load_texture("decidueye_idle", "assets/decidueye/idle.png", decidueye_render_scale);
    this->load_texture("decidueye_run", "assets/decidueye/run.png", decidueye_render_scale);
    this->load_texture("decidueye_jump", "assets/decidueye/jump.png", decidueye_render_scale);
    this->load_texture("decidueye_attack", "assets/decidueye/attack.png", decidueye_render_scale);
    this->load_texture("decidueye_attack_air", "assets/decidueye/attack_air.png", decidueye_render_scale);
    this->load_texture("decidueye_hurt", "assets/decidueye/hurt.png", decidueye_render_scale);
    this->load_texture("decidueye_dash", "assets/decidueye/dash.png", decidueye_render_scale);

    this->load_texture("tiles_debug", "assets/tiles-debug.png", Vector2{ 1.0F, 1.0F });
}

void TextureEngine::TextureEngine::load_texture(
    const std::string& texture_id, const std::string& texture_input_file, Vector2 render_scale
) {
    // if the texture is already present, then don't load it again
    auto iter = m_textures.find(texture_id);
    if (iter != m_textures.end()) {
        return;
    }
    m_textures[texture_id] = GameTexture{
        .texture = LoadTexture(texture_input_file.c_str()),
        .render_scale = render_scale,
    };
}
TextureEngine::GameTexture
TextureEngine::TextureEngine::get_texture(const std::string& texture_id) const {
    return m_textures.at(texture_id);
}
void TextureEngine::TextureEngine::unload_textures() const {
    for (const auto& [key, value] : m_textures) {
        UnloadTexture(value.texture);
    }
}
