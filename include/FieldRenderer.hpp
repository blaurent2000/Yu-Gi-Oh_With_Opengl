
#pragma once
#include <glad/gl.h>
#include <glm/glm.hpp>
#include "Field.hpp"
#include "CardRenderer.hpp"

class FieldRenderer {
public:
    FieldRenderer(unsigned int shaderProgram, unsigned int VAO, Field& field, CardRenderer& cardRenderer);
    void draw(const glm::mat4& projection, int width, int height);

private:
    void drawZone(const glm::mat4& projection, const glm::vec2& position,
                  const glm::vec2& size, const glm::vec3& color);

    unsigned int shaderProgram;
    unsigned int VAO;
    Field& field;
    CardRenderer& cardRenderer;
    int modelLocation;
    int projectionLocation;
    int colorLocation;
};