#include "texture_engine.hpp"

void TextureEngine::TextureEngine::setup() {
    this->load_texture("cowboy_idle", "assets/cowboy/idle.png");
    this->load_texture("cowboy_run", "assets/cowboy/run.png");
    this->load_texture("cowboy_jump", "assets/cowboy/jump.png");
    this->load_texture("cowboy_attack", "assets/cowboy/sword_slash_vertical.png");
    this->load_texture("cowboy_attack_air", "assets/cowboy/horizontal_air_slash.png");
    this->load_texture("cowboy_hurt", "assets/cowboy/hurt.png");
    this->load_texture("cowboy_dash", "assets/cowboy/dash.png");

    this->load_texture("decidueye_idle", "assets/decidueye/idle.png");
    this->load_texture("decidueye_run", "assets/decidueye/run.png");
    this->load_texture("decidueye_jump", "assets/decidueye/jump.png");
    this->load_texture("decidueye_attack", "assets/decidueye/attack.png");
    this->load_texture("decidueye_attack_air", "assets/decidueye/attack_air.png");
    this->load_texture("decidueye_hurt", "assets/decidueye/hurt.png");
    this->load_texture("decidueye_dash", "assets/decidueye/dash.png");

    this->load_texture("tiles_debug", "assets/tiles-debug.png");
}

void TextureEngine::TextureEngine::load_texture(const std::string& texture_id, const std::string& texture_input_file) {
    // if the texture is already present, then don't load it again
    auto iter = m_textures.find(texture_id);
    if (iter != m_textures.end()) {
        return;
    }
    m_textures[texture_id] = LoadTexture(texture_input_file.c_str());
}
Texture2D TextureEngine::TextureEngine::get_texture(const std::string& texture_id) const {
    return m_textures.at(texture_id);
}
void TextureEngine::TextureEngine::unload_textures() const {
    for (const auto& [key, value] : m_textures) {
        UnloadTexture(value);
    }
}
