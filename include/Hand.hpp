
#pragma once

#include "Card.hpp"
#include <vector>

class Hand {
public:
    void addCard(const Card& card);
    void removeCard(int index);
    Card* getCard(int index);
    const std::vector<Card>& getCards() const;
    int size() const;

private:
    std::vector<Card> cards;
};
