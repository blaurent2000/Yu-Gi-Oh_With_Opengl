
#include "Card.hpp"
#include "CardRenderer.hpp"
#include "PhaseRenderer.hpp"
#include "TextureLibrary.hpp"
#include "FieldRenderer.hpp"
#include "Field.hpp"
#include "Hand.hpp"
#include "TextRenderer.hpp"
#include "GameManager.hpp"
#include "Deck.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include <iostream>
#include <vector>

glm::mat4 projection;

void framebufferSizeCallback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
    projection = glm::ortho(
        0.0f, static_cast<float>(width),
        static_cast<float>(height), 0.0f,
        -1.0f, 1.0f
    );
}

unsigned int loadTexture(const char* path) {
    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 4);

    if (!data) {
        std::cerr << "Erreur : impossible de charger " << path << '\n';
        return 0;
    }

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, data
    );
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Erreur : impossible d'initialiser GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Card Game", nullptr, nullptr);

    if (!window) {
        std::cerr << "Erreur : impossible de créer la fenêtre\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cerr << "Erreur : impossible de charger GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwMaximizeWindow(window);
    glfwPollEvents();
    glfwShowWindow(window);
    glfwPollEvents();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    framebufferSizeCallback(window, width, height);

    std::cout << "OpenGL : " << glGetString(GL_VERSION) << '\n';

    float vertices[] = {
        0, 0, 0, 0, 0,
        200, 0, 0, 1, 0,
        200, 300, 0, 1, 1,
        0, 0, 0, 0, 0,
        200, 300, 0, 1, 1,
        0, 300, 0, 0, 1
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTexCoord;

        uniform mat4 model;
        uniform mat4 projection;

        out vec2 TexCoord;

        void main() {
            gl_Position = projection * model * vec4(aPos, 1.0);
            TexCoord = aTexCoord;
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec2 TexCoord;
        out vec4 FragColor;
        uniform sampler2D texture1;

        void main() {
            FragColor = texture(texture1, TexCoord);
        }
    )";

    const char* fieldShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec3 zoneColor;

        void main() {
            FragColor = vec4(zoneColor, 1.0);
        }
    )";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    unsigned int fieldShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fieldShader, 1, &fieldShaderSource, nullptr);
    glCompileShader(fieldShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    unsigned int fieldShaderProgram = glCreateProgram();
    glAttachShader(fieldShaderProgram, vertexShader);
    glAttachShader(fieldShaderProgram, fieldShader);
    glLinkProgram(fieldShaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fieldShader);

    // Ces ressources restent accessibles après la destruction des renderers.
    unsigned int baseTexture = 0;
    unsigned int backTexture = 0;
    unsigned int starTexture = 0;
    std::vector<unsigned int> phasesTextures;

    {
        Field field;

        Card blueEyes(
            "Blue-Eyes White Dragon", MONSTER, 3000, 2500, 12,
            "Un dragon légendaire d'une puissance incroyable."
        );

        Card darkMagician(
            "Dark Magician", MONSTER, 2500, 2100, 7,
            "Le magicien ultime en termes d'attaque et de défense."
        );

        Hand hand;
        hand.addCard(blueEyes);
        hand.addCard(darkMagician);

        baseTexture = loadTexture("textures/full_card.jpg");
        backTexture = loadTexture("textures/card_back.jpg");
        starTexture = loadTexture("textures/star.png");

        phasesTextures.push_back(loadTexture("textures/phases/d.png"));
        phasesTextures.push_back(loadTexture("textures/phases/sb.png"));
        phasesTextures.push_back(loadTexture("textures/phases/m1.png"));
        phasesTextures.push_back(loadTexture("textures/phases/btl.png"));
        phasesTextures.push_back(loadTexture("textures/phases/m2.png"));
        phasesTextures.push_back(loadTexture("textures/phases/end.png"));

        phasesTextures.push_back(loadTexture("textures/phases/d_current.png"));
        phasesTextures.push_back(loadTexture("textures/phases/sb_current.png"));
        phasesTextures.push_back(loadTexture("textures/phases/m1_current.png"));
        phasesTextures.push_back(loadTexture("textures/phases/btl_current.png"));
        phasesTextures.push_back(loadTexture("textures/phases/m2_current.png"));
        phasesTextures.push_back(loadTexture("textures/phases/end_current.png"));

        TextureLibrary textureLibrary;
        textureLibrary.load("Blue-Eyes White Dragon");
        textureLibrary.load("Dark Magician");

        TextRenderer textRenderer("shaders/text.vert", "shaders/text.frag");
        textRenderer.loadFont(
            FontType::Regular, "fonts/Matrix Regular.otf", 48
        );
        textRenderer.loadFont(
            FontType::SmallCaps, "fonts/Matrix Regular Small Caps.ttf", 48
        );

        CardRenderer renderer(
            shaderProgram, VAO, baseTexture, backTexture, starTexture,
            textureLibrary, &textRenderer
        );

        FieldRenderer fieldRenderer(fieldShaderProgram, VAO, field, renderer);

        GameManager gameManager;
        PhaseRenderer phaseRenderer(
            shaderProgram, VAO, phasesTextures, textureLibrary, gameManager
        );

        glm::vec2 phasePosition(515.0f, 355.0f);
        glm::vec2 phaseSize(70.0f, 70.0f);

        static bool wasMousePressed = false;

        Deck deck;
        deck.initDeck("./deck/deck1");

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            bool isMousePressed =
                glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

            if (isMousePressed && !wasMousePressed) {
                std::cout << "mouse clic : x=" << mouseX << " y=" << mouseY << '\n';
                for (int i = 0; i < 6; i++) {
                    glm::vec2 phasePos = phasePosition + glm::vec2(100.0f * i, 0.0f);

                    if (mouseX >= phasePos.x && mouseX <= phasePos.x + phaseSize.x &&
                        mouseY >= phasePos.y && mouseY <= phasePos.y + phaseSize.y) {
                        std::cout << "Phase cliquée : " << i << '\n';
                        if (i == static_cast<int>(gameManager.getCurrentPhase()) + 1) {
                            gameManager.setCurrentPhase(static_cast<Phase>(i));
                        }
                        if (gameManager.getCurrentPhase() == Phase::End) {
                            gameManager.setCurrentPhase(Phase::Draw);
                            gameManager.setCurrentPlayer(
                                gameManager.getCurrentPlayer() == Player::Player1
                                    ? Player::Player2
                                    : Player::Player1
                            );
                        }
                    }
                }
            }
            wasMousePressed = isMousePressed;

            glClearColor(0.04f, 0.04f, 0.08f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            fieldRenderer.draw(projection, width, height);
            phaseRenderer.drawPhases(projection, phasePosition, phaseSize);

            glm::vec2 handCardSize(100.0f, 150.0f);
            float overlap = 35.0f;

            float startX = width - 30.0f - handCardSize.x
                         - (hand.size() - 1) * overlap;
            float handY = height - handCardSize.y - 20.0f;

            int hoveredCard = -1;

            for (int i = static_cast<int>(hand.size()) - 1; i >= 0; i--) {
                float x = startX + i * overlap;

                if (mouseX >= x && mouseX <= x + handCardSize.x &&
                    mouseY >= handY && mouseY <= handY + handCardSize.y) {
                    hoveredCard = i;
                    break;
                }
            }

            for (int i = 0; i < hand.size(); i++) {
                glm::vec2 position(startX + i * overlap, handY);
                renderer.draw(*hand.getCard(i), projection, position, handCardSize);
            }

            if (hoveredCard != -1) {
                renderer.draw(
                    *hand.getCard(hoveredCard),
                    projection,
                    glm::vec2(30.0f, height / 2.0f - 225.0f),
                    glm::vec2(300.0f, 450.0f)
                );
            }

            textRenderer.drawText(
                FontType::SmallCaps,
                "Blue-Eyes White Dragon",
                50.0f, 100.0f,
                0.5f,
                glm::vec3(1.0f, 1.0f, 1.0f),
                projection
            );

            glfwSwapBuffers(window);
        }

    } // TextRenderer et les autres objets sont détruits ici.

    // Le contexte OpenGL existe toujours à cet endroit.
    glDeleteTextures(1, &baseTexture);
    glDeleteTextures(1, &backTexture);
    glDeleteTextures(1, &starTexture);

    for (unsigned int texture : phasesTextures) {
        if (texture)
            glDeleteTextures(1, &texture);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(fieldShaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
