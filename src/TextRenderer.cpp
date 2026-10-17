
#include "TextRenderer.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

TextRenderer::TextRenderer(
    const std::string& vertexPath,
    const std::string& fragmentPath
) : shaderProgram(0), VAO(0), VBO(0), ft(nullptr) {
    shaderProgram = createShaderProgram(vertexPath, fragmentPath);

    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Erreur : initialisation de FreeType impossible\n";
        return;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

TextRenderer::~TextRenderer() {
    for (auto& font : fonts) {
        for (auto& character : font.second) {
            glDeleteTextures(1, &character.second.textureID);
        }
    }

    if (VAO)
        glDeleteVertexArrays(1, &VAO);
    if (VBO)
        glDeleteBuffers(1, &VBO);
    if (shaderProgram)
        glDeleteProgram(shaderProgram);
    if (ft)
        FT_Done_FreeType(ft);
}

GLuint TextRenderer::createShaderProgram(
    const std::string& vertexPath,
    const std::string& fragmentPath
) {
    auto readFile = [](const std::string& path) {
        std::ifstream file(path);
        if (!file) {
            std::cerr << "Erreur : impossible de lire " << path << '\n';
            return std::string{};
        }

        std::stringstream stream;
        stream << file.rdbuf();
        return stream.str();
    };

    std::string vertexCode = readFile(vertexPath);
    std::string fragmentCode = readFile(fragmentPath);

    if (vertexCode.empty() || fragmentCode.empty())
        return 0;

    auto compileShader = [](GLenum type, const std::string& source) {
        GLuint shader = glCreateShader(type);
        const char* code = source.c_str();
        glShaderSource(shader, 1, &code, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            char log[512];
            glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
            std::cerr << "Erreur de compilation du shader :\n" << log << '\n';
            glDeleteShader(shader);
            return GLuint(0);
        }

        return shader;
    };

    GLuint vertex = compileShader(GL_VERTEX_SHADER, vertexCode);
    GLuint fragment = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

    if (!vertex || !fragment) {
        if (vertex) glDeleteShader(vertex);
        if (fragment) glDeleteShader(fragment);
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        char log[512];
        glGetProgramInfoLog(program, sizeof(log), nullptr, log);
        std::cerr << "Erreur de liaison du shader :\n" << log << '\n';
        glDeleteProgram(program);
        program = 0;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return program;
}

bool TextRenderer::loadFont(
    FontType type,
    const std::string& path,
    unsigned int fontSize
) {
    if (!ft || !shaderProgram)
        return false;

    FT_Face face;

    if (FT_New_Face(ft, path.c_str(), 0, &face)) {
        std::cerr << "Erreur : impossible de charger la police " << path << '\n';
        return false;
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    auto& characters = fonts[type];

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "Impossible de charger le caractère " << c << '\n';
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0, GL_RED, GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };

        characters[static_cast<char>(c)] = character;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    FT_Done_Face(face);

    return true;
}

void TextRenderer::drawText(
    FontType type,
    const std::string& text,
    float x, float y,
    float scale,
    const glm::vec3& color,
    const glm::mat4& projection
) {
    auto font = fonts.find(type);

    if (font == fonts.end() || !shaderProgram)
        return;

    glUseProgram(shaderProgram);

    glUniform3f(
        glGetUniformLocation(shaderProgram, "textColor"),
        color.r, color.g, color.b
    );

    glUniformMatrix4fv(
        glGetUniformLocation(shaderProgram, "projection"),
        1, GL_FALSE, &projection[0][0]
    );

    glUniform1i(glGetUniformLocation(shaderProgram, "text"), 0);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    GLboolean blendWasEnabled = glIsEnabled(GL_BLEND);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (unsigned char c : text) {
        auto it = font->second.find(static_cast<char>(c));

        if (it == font->second.end())
            continue;

        const Character& ch = it->second;

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;
        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        float vertices[6][4] = {
            { xpos,     ypos + h, 0.0f, 1.0f },
            { xpos,     ypos,     0.0f, 0.0f },
            { xpos + w, ypos,     1.0f, 0.0f },

            { xpos,     ypos + h, 0.0f, 1.0f },
            { xpos + w, ypos,     1.0f, 0.0f },
            { xpos + w, ypos + h, 1.0f, 1.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (!blendWasEnabled)
        glDisable(GL_BLEND);
}

void TextRenderer::drawWrappedText(
    FontType type,
    const std::string& text,
    float x, float y,
    float maxWidth,
    float scale,
    const glm::vec3& color,
    const glm::mat4& projection
) {
    auto font = fonts.find(type);
    if (font == fonts.end() || !shaderProgram) return;

    float startX = x;
    float lineHeight = 48.0f * scale * 1.2f;
    std::string word;
    std::string line;

    auto getWidth = [&](const std::string& str) {
        float width = 0.0f;
        for (unsigned char c : str) {
            auto it = font->second.find(static_cast<char>(c));
            if (it != font->second.end())
                width += (it->second.advance >> 6) * scale;
        }
        return width;
    };

    std::istringstream stream(text);
    while (stream >> word) {
        std::string test = line.empty() ? word : line + " " + word;

        if (getWidth(test) > maxWidth && !line.empty()) {
            drawText(type, line, startX, y, scale, color, projection);
            line = word;
            y += lineHeight;
        } else {
            line = test;
        }
    }

    if (!line.empty())
        drawText(type, line, startX, y, scale, color, projection);
}