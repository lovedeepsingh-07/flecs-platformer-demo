#include "texture_engine.hpp"

void TextureEngine::TextureEngine::setup() {
    auto cowboy_render_scale = Vector2{ 1.0F, 1.0F };
    this->load_texture("cowboy_air_dash", "assets/cowboy/air_dash.png", cowboy_render_scale);
    this->load_texture("cowboy_burst", "assets/cowboy/burst.png", cowboy_render_scale);
    this->load_texture("cowboy_getup", "assets/cowboy/getup.png", cowboy_render_scale);
    this->load_texture("cowboy_horizontal_air_slash", "assets/cowboy/horizontal_air_slash.png", cowboy_render_scale);
    this->load_texture("cowboy_hurt", "assets/cowboy/hurt.png", cowboy_render_scale);
    this->load_texture("cowboy_idle", "assets/cowboy/idle.png", cowboy_render_scale);
    this->load_texture("cowboy_jump", "assets/cowboy/jump.png", cowboy_render_scale);
    this->load_texture("cowboy_knockdown", "assets/cowboy/knockdown.png", cowboy_render_scale);
    this->load_texture("cowboy_run", "assets/cowboy/run.png", cowboy_render_scale);
    this->load_texture("cowboy_shield_high", "assets/cowboy/shield_high.png", cowboy_render_scale);
    this->load_texture("cowboy_sword_slash_horizontal", "assets/cowboy/sword_slash_horizontal.png", cowboy_render_scale);
    this->load_texture("cowboy_sword_slash_horizontal_big", "assets/cowboy/sword_slash_horizontal_big.png", cowboy_render_scale);
    this->load_texture("cowboy_sword_slash_horizontal_up", "assets/cowboy/sword_slash_horizontal_up.png", cowboy_render_scale);
    this->load_texture("cowboy_sword_slash_vertical", "assets/cowboy/sword_slash_vertical.png", cowboy_render_scale);
    this->load_texture("cowboy_taunt", "assets/cowboy/taunt.png", cowboy_render_scale);
    this->load_texture("cowboy_teleport", "assets/cowboy/teleport.png", cowboy_render_scale);
    this->load_texture("cowboy_triple_ass_smacker_1", "assets/cowboy/triple_ass_smacker_1.png", cowboy_render_scale);
    this->load_texture("cowboy_triple_ass_smacker_2", "assets/cowboy/triple_ass_smacker_2.png", cowboy_render_scale);
    this->load_texture("cowboy_triple_ass_smacker_3", "assets/cowboy/triple_ass_smacker_3.png", cowboy_render_scale);
    this->load_texture("cowboy_vertical_air_slash", "assets/cowboy/vertical_air_slash.png", cowboy_render_scale);
    this->load_texture("cowboy_dash", "assets/cowboy/dash.png", cowboy_render_scale);

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
