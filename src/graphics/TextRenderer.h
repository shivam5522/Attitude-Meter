#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "../config.h"
#include "Shader.h"

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2 Size;        // Size of glyph
    glm::ivec2 Bearing;     // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Offset to advance to next glyph
};

class TextRenderer {
public:
    TextRenderer(const std::string& fontPath, unsigned int fontSize);
    void renderText(Shader& shader, const std::string& text, float x, float y, float scale, glm::vec4 color);

private:
    std::map<char, Character> Characters;
    unsigned int VAO, VBO;

    void loadFont(const std::string& fontPath, unsigned int fontSize);
};

#endif // TEXT_RENDERER_H