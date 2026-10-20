
#pragma once

#include "TextureLibrary.hpp"
#include "GameManager.hpp"
#include <vector>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/vec2.hpp> 

class PhaseRenderer {
    public:
        PhaseRenderer(
            unsigned int shaderProgram,
            unsigned int VAO,
            std::vector<unsigned int> phasesTextures,
            TextureLibrary& textureLibrary,
            GameManager& gameManager
        );
        void drawPhases(
            const glm::mat4& projection,
            const glm::vec2& position,
            const glm::vec2& size
        );
        void drawPhase(
            unsigned int texture,
            const glm::mat4& projection,
            const glm::vec2& position,
            const glm::vec2& size
        );
    private:
        unsigned int shaderProgram;
        unsigned int VAO;

        std::vector<unsigned int> phasesTextures;

        TextureLibrary& textureLibrary;
        GameManager&    gameManager;

        int modelLocation;
        int projectionLocation;
        int textureLocation;
};