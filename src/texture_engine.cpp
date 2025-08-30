#include "texture_engine.hpp"

void TextureEngine::TextureEngine::setup() {
    this->load_texture("player_idle", "assets/player/idle.png");
    this->load_texture("player_run", "assets/player/run.png");
    this->load_texture("player_jump", "assets/player/jump.png");
    this->load_texture("player_attack", "assets/player/sword_slash_vertical.png");
    this->load_texture("player_attack_air", "assets/player/horizontal_air_slash.png");
    this->load_texture("player_hurt", "assets/player/hurt.png");
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
void TextureEngine::TextureEngine::unload_textures() {
    for (const auto& [key, value] : m_textures) {
        UnloadTexture(value);
    }
}
