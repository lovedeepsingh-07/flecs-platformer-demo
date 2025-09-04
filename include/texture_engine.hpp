#include <raylib.h>
#include <string>
#include <unordered_map>

namespace TextureEngine {
    struct GameTexture {
        Texture2D texture;
        Vector2 render_scale;
    };
    class TextureEngine {
      private:
        std::unordered_map<std::string, GameTexture> m_textures;

      public:
        void setup();
        void load_texture(const std::string& texture_id, const std::string& texture_input_file, Vector2 render_scale);
        GameTexture get_texture(const std::string& texture_id) const;
        void unload_textures() const;
    };
}
