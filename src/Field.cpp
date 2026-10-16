
#include "Field.hpp"

Field::Field() {
    float x[] = {0, 130, 260, 390, 520, 650, 780};
    float y[] = {0, 190, 380, 570};

    // Adversaire : Deck, Magies/Pièges, Extra Deck
    zones.emplace_back(DECK_ZONE, glm::vec2(x[0], y[0]));
    for (int i = 0; i < 5; i++)
        zones.emplace_back(SPELL_ZONE, glm::vec2(x[i + 1], y[0]));
    zones.emplace_back(EXTRA_DECK_ZONE, glm::vec2(x[6], y[0]));

    // Adversaire : Cimetière, monstres, terrain
    zones.emplace_back(GRAVEYARD_ZONE, glm::vec2(x[0], y[1]));
    for (int i = 0; i < 5; i++)
        zones.emplace_back(MONSTER_ZONE, glm::vec2(x[i + 1], y[1]));
    zones.emplace_back(FIELD_ZONE, glm::vec2(x[6], y[1]));

    // Joueur : terrain, monstres, cimetière
    zones.emplace_back(FIELD_ZONE, glm::vec2(x[0], y[2]));
    for (int i = 0; i < 5; i++)
        zones.emplace_back(MONSTER_ZONE, glm::vec2(x[i + 1], y[2]));
    zones.emplace_back(GRAVEYARD_ZONE, glm::vec2(x[6], y[2]));

    // Joueur : Extra Deck, Magies/Pièges, Deck
    zones.emplace_back(EXTRA_DECK_ZONE, glm::vec2(x[0], y[3]));
    for (int i = 0; i < 5; i++)
        zones.emplace_back(SPELL_ZONE, glm::vec2(x[i + 1], y[3]));
    zones.emplace_back(DECK_ZONE, glm::vec2(x[6], y[3]));
}

FieldZone* Field::getZone(int index) {
    if (index < 0 || index >= static_cast<int>(zones.size()))
        return nullptr;
    return &zones[index];
}

bool Field::placeCard(int index, Card* card) {
    FieldZone* zone = getZone(index);
    if (!zone || !card || !zone->isEmpty())
        return false;
    zone->setCard(card);
    return true;
}

void Field::removeCard(int index) {
    FieldZone* zone = getZone(index);
    if (zone) zone->setCard(nullptr);
}
