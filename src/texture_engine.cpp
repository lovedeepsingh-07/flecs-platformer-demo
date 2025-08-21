#include "texture_engine.hpp"

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
