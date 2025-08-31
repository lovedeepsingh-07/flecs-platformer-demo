#include <raylib.h>
#include <string>
#include <unordered_map>

namespace TextureEngine {
    class TextureEngine {
      private:
        std::unordered_map<std::string, Texture2D> m_textures;

      public:
        void setup();
        void load_texture(const std::string& texture_id, const std::string& texture_input_file);
        Texture2D get_texture(const std::string& texture_id) const;
        void unload_textures() const;
    };
}
