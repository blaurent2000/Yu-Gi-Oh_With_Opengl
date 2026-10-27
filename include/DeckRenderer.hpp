
#pragma once

#include "CardRenderer.hpp"
#include "Deck.hpp"

#include <glm/gtc/matrix_transform.hpp>

class DeckRenderer {
    public:
        DeckRenderer(
            CardRenderer& cardRenderer,
            Deck& deck1,
            Deck& deck2
        );
        void drawDecks(
            glm::mat4& projection,
            float width,
            float height
        );
    private:
        CardRenderer& cardRenderer;
        Deck& deck1;
        Deck& deck2;
};