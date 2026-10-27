
#include "DeckRenderer.hpp"


DeckRenderer::DeckRenderer(
            CardRenderer& cardRenderer,
            Deck& deck1,
            Deck& deck2
        )
        :
        cardRenderer(cardRenderer),
        deck1(deck1),
        deck2(deck2)
{
}

void DeckRenderer::drawDecks(
    glm::mat4& projection,
    float width,
    float height
)
{
    glm::vec2 offset(
        (width - 900.0f) / 2.0f,
        (height - 750.0f) / 2.0f
    );

    glm::vec2 cardSize(100.0f, 150.0f);
    glm::vec2 stackOffset(1.0f, 1.0f);

    glm::vec2 position1 = glm::vec2(780.0f, 590.0f) + offset;
    glm::vec2 position2 = glm::vec2(0.0f, -20.0f) + offset;

    for (int i = 0; i < deck1.getDeckSize(); i++) {
        cardRenderer.drawBack(
            projection,
            position1 + stackOffset * static_cast<float>(i),
            cardSize
        );
    }

    for (int i = 0; i < deck2.getDeckSize(); i++) {
        cardRenderer.drawBack(
            projection,
            position2 + stackOffset * static_cast<float>(i),
            cardSize
        );
    }
}