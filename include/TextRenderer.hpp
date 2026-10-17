
#pragma once

#include <string>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/gl.h>
#include <glm/glm.hpp>

enum class FontType {
    Regular,
    SmallCaps
};

struct Character {
    GLuint textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class TextRenderer {
public:
    TextRenderer(
        const std::string& vertexPath,
        const std::string& fragmentPath
    );
    ~TextRenderer();

    void drawWrappedText(
        FontType type,
        const std::string& text,
        float x, float y,
        float maxWidth,
        float scale,
        const glm::vec3& color,
        const glm::mat4& projection
    );

    bool loadFont(FontType type, const std::string& path, unsigned int fontSize);

    void drawText(
        FontType type,
        const std::string& text,
        float x, float y,
        float scale,
        const glm::vec3& color,
        const glm::mat4& projection
    );

private:
    GLuint shaderProgram;
    GLuint VAO, VBO;
    FT_Library ft;

    std::map<FontType, std::map<char, Character>> fonts;

    GLuint createShaderProgram(
        const std::string& vertexPath,
        const std::string& fragmentPath
    );
};
