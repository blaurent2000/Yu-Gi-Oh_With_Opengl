#include "CardRenderer.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

CardRenderer::CardRenderer(
    unsigned int shaderProgram,
    unsigned int VAO,
    unsigned int baseTexture,
    unsigned int backTexture,
    unsigned int starTexture,
    TextureLibrary& textureLibrary,
    TextRenderer* textRenderer
)
    : shaderProgram(shaderProgram),
      VAO(VAO),
      baseTexture(baseTexture),
      backTexture(backTexture),
      starTexture(starTexture),
      textureLibrary(textureLibrary),
      textRenderer(textRenderer)
{
    modelLocation = glGetUniformLocation(shaderProgram, "model");
    projectionLocation = glGetUniformLocation(shaderProgram, "projection");
    textureLocation = glGetUniformLocation(shaderProgram, "texture1");
}

void CardRenderer::drawTexture(
    unsigned int texture,
    const glm::mat4& projection,
    const glm::vec2& position,
    const glm::vec2& size
)
{
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(
        model,
        glm::vec3(position, 0.0f)
    );

    model = glm::scale(
        model,
        glm::vec3(
            size.x / 200.0f,
            size.y / 300.0f,
            1.0f
        )
    );

    glUseProgram(shaderProgram);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glUniform1i(textureLocation, 0);

    glUniformMatrix4fv(
        projectionLocation,
        1,
        GL_FALSE,
        glm::value_ptr(projection)
    );

    glUniformMatrix4fv(
        modelLocation,
        1,
        GL_FALSE,
        glm::value_ptr(model)
    );

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void CardRenderer::draw(
    const Card& card,
    const glm::mat4& projection,
    const glm::vec2& position,
    const glm::vec2& size
)
{
    if (card.isFaceDown()) {
        drawTexture(
            backTexture,
            projection,
            position,
            size
        );

        return;
    }

    // 1. Dessine la carte de base
    drawTexture(
        baseTexture,
        projection,
        position,
        size
    );

    // 2. Récupère l'image du monstre
    unsigned int monsterTexture =
        textureLibrary.get(card.getName());

    if (monsterTexture != 0) {
        // 3. Dessine l'image du monstre dans la zone d'illustration
        drawTexture(
            monsterTexture,
            projection,
            position + size * glm::vec2(22.0f / 200.0f, 54.0f / 300.0f),
            size * glm::vec2(155.0f / 200.0f, 160.0f / 300.0f)
        );
    }


    int lvlMax = card.getLvl();
    for (int lvl = 0; lvl < lvlMax; lvl++) {
        drawTexture(
            starTexture,
            projection,
            position + size * glm::vec2((1650.0f - (lvl * 130)) / 2000.0f, (370.0f / 3000.0f)),
            size * glm::vec2(130.0f / 2000.0f, 130.0f / 3000.0f)
        );
    }

            // Affiche le nom au-dessus des étoiles
        textRenderer->drawText(
            FontType::SmallCaps,
            card.getName(),
            position.x + size.x * 0.09f,
            position.y + size.y * 0.065f,
            size.x / 450.0f,
            glm::vec3(0.0f, 0.0f, 0.0f),
            projection
        );

        textRenderer->drawWrappedText(
            FontType::SmallCaps,
            card.getDescription(),
            position.x + size.x * 0.09f,
            position.y + size.y * 0.77f,
            size.x * 0.84f,
            size.x / 800.0f,
            glm::vec3(0.0f, 0.0f, 0.0f),
            projection
        );

        textRenderer->drawText(
            FontType::SmallCaps,
            std::to_string(card.getAttack()),
            position.x + size.x * 0.62f,
            position.y + size.y * 0.915f,
            size.x / 800.0f,
            glm::vec3(0.0f, 0.0f, 0.0f),
            projection
        );

        textRenderer->drawText(
            FontType::SmallCaps,
            std::to_string(card.getDefense()),
            position.x + size.x * 0.82f,
            position.y + size.y * 0.915f,
            size.x / 800.0f,
            glm::vec3(0.0f, 0.0f, 0.0f),
            projection
        );
}