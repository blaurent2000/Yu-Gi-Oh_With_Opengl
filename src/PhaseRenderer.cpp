


#include "PhaseRenderer.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

PhaseRenderer::PhaseRenderer(
    unsigned int shaderProgram,
    unsigned int VAO,
    std::vector<unsigned int> phasesTextures,
    TextureLibrary& textureLibrary,
    GameManager& gameManager
) :
    shaderProgram(shaderProgram),
    VAO(VAO),
    phasesTextures(phasesTextures),
    textureLibrary(textureLibrary),
    gameManager(gameManager)
{
    modelLocation = glGetUniformLocation(shaderProgram, "model");
    projectionLocation = glGetUniformLocation(shaderProgram, "projection");
    textureLocation = glGetUniformLocation(shaderProgram, "texture1");
}

// void PhaseRenderer::drawPhase(
//     unsigned int texture,
//     const glm::mat4& projection,
//     const glm::vec2& position,
//     const glm::vec2& size
// );

void PhaseRenderer::drawPhase(
    unsigned int texture,
    const glm::mat4& projection,
    const glm::vec2& position,
    const glm::vec2& size
) {
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(
        model,
        glm::vec3(position, 0.0f)
    );

    model = glm::scale(
        model,
        glm::vec3(
            size.x/200,
            size.y/200,
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

void PhaseRenderer::drawPhases(
    const glm::mat4& projection,
    const glm::vec2& position,
    const glm::vec2& size
) {

    glm::vec2 gap(100.0f, 0.0f);
    int phaseIndex = static_cast<int>(gameManager.getCurrentPhase());
    for (int i = 0; i < 6; i++) {
        if (i == phaseIndex) {
            drawPhase(
                phasesTextures[i + 6],
                projection,
                position + gap * static_cast<float>(i),
                size
            );
        } else {
            drawPhase(
                phasesTextures[i],
                projection,
                position + gap * static_cast<float>(i),
                size
            );
        }
    }

}