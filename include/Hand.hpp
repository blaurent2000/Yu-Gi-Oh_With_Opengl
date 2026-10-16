#pragma once
#include "Card.hpp"
#include <vector>

class Hand {
public:
    void addCard(Card* card);
    void removeCard(int index);
    Card* getCard(int index) const;
    const std::vector<Card*>& getCards() const { return cards; }
    int size() const { return cards.size(); }

private:
    std::vector<Card*> cards;
};