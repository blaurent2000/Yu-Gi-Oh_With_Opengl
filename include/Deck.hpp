
#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "Card.hpp"
#include "Hand.hpp"

class Deck {
public:
    void initDeck(const std::string& path);
    void drawCard(Hand& hand);
    void addCard(const Card& card);
    void shuffleDeck();
    int getDeckSize() const;

private:
    std::vector<Card> deck;
};
