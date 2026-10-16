
#pragma once
#include "Card.hpp"
#include <glm/vec2.hpp>

enum ZoneType {
    MONSTER_ZONE,
    SPELL_ZONE,
    DECK_ZONE,
    GRAVEYARD_ZONE,
    EXTRA_DECK_ZONE,
    FIELD_ZONE
};

class FieldZone {
public:
    FieldZone(ZoneType type, const glm::vec2& position)
        : type(type), position(position), card(nullptr) {}

    ZoneType getType() const { return type; }
    glm::vec2 getPosition() const { return position; }
    Card* getCard() const { return card; }
    void setCard(Card* newCard) { card = newCard; }
    bool isEmpty() const { return card == nullptr; }

private:
    ZoneType type;
    glm::vec2 position;
    Card* card;
};