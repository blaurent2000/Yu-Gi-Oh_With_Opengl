
#include "FieldRenderer.hpp"
#include <glm/gtc/matrix_transform.hpp>

FieldRenderer::FieldRenderer(unsigned int shaderProgram, unsigned int VAO,
                             Field& field, CardRenderer& cardRenderer)
    : shaderProgram(shaderProgram), VAO(VAO), field(field), cardRenderer(cardRenderer) {
    modelLocation = glGetUniformLocation(shaderProgram, "model");
    projectionLocation = glGetUniformLocation(shaderProgram, "projection");
    colorLocation = glGetUniformLocation(shaderProgram, "zoneColor");
}

void FieldRenderer::drawZone(const glm::mat4& projection, const glm::vec2& position,
                             const glm::vec2& size, const glm::vec3& color) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(size.x / 200.0f, size.y / 300.0f, 1.0f));

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
    glUniform3fv(colorLocation, 1, &color[0]);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void FieldRenderer::draw(const glm::mat4& projection, int width, int height) {
    glm::vec2 zoneSize(120.0f, 180.0f);
    glm::vec2 offset((width - 900.0f) / 2.0f, (height - 750.0f) / 2.0f);

    for (const FieldZone& zone : field.getZones()) {
        glm::vec2 position = zone.getPosition() + offset;
        glm::vec3 color;

        switch (zone.getType()) {
            case MONSTER_ZONE: color = glm::vec3(0.05f, 0.25f, 0.75f); break;
            case SPELL_ZONE: color = glm::vec3(0.45f, 0.10f, 0.65f); break;
            case FIELD_ZONE: color = glm::vec3(0.1f, 0.5f, 0.3f); break;
            default: color = glm::vec3(0.3f, 0.3f, 0.3f); break;
        }

        drawZone(projection, position, zoneSize, color);

        if (!zone.isEmpty())
            cardRenderer.draw(*zone.getCard(), projection, position, zoneSize);
    }
}


