#pragma once

#include "Card.hpp"
#include "TextureLibrary.hpp"
#include "TextRenderer.hpp"

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>

class CardRenderer
{
public:
    CardRenderer(
        unsigned int shaderProgram,
        unsigned int VAO,
        unsigned int baseTexture,
        unsigned int backTexture,
        unsigned int starTexture,
        TextureLibrary& textureLibrary,
        TextRenderer* textRenderer
    );

    void draw(const Card& card, const glm::mat4& projection,
          const glm::vec2& position,
          const glm::vec2& size = glm::vec2(100.0f, 150.0f));

private:
    void drawTexture(
        unsigned int texture,
        const glm::mat4& projection,
        const glm::vec2& position,
        const glm::vec2& size
    );

    unsigned int shaderProgram;
    unsigned int VAO;

    unsigned int baseTexture;
    unsigned int backTexture;
    unsigned int starTexture;

    TextureLibrary& textureLibrary;
    TextRenderer* textRenderer;

    int modelLocation;
    int projectionLocation;
    int textureLocation;
};