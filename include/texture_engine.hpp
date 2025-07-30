#include "raylib.h"
#include <string>
#include <unordered_map>

class TextureEngine {
  private:
    std::unordered_map<std::string, Texture2D> m_textures;

  public:
    void load_texture(const std::string& texture_name, const std::string& texture_input_file) {
        // if the texture is already present, then don't load it again
        auto iter = m_textures.find(texture_name);
        if (iter != m_textures.end()) {
            return;
        }
        m_textures[texture_name] = LoadTexture(texture_input_file.c_str());
    }
    Texture2D get_texture(const std::string& texture_name) {
        return m_textures[texture_name];
    }
    void unload_textures() {
        for (const auto& [key, value] : m_textures) {
            UnloadTexture(value);
        }
    }
};
