
#include "Deck.hpp"
#include <iostream>

void Deck::initDeck(const std::string& path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "Impossible d'ouvrir le fichier : "
                  << path << std::endl;
        return;
    }

    std::string str;

    while (std::getline(file, str)) {
        std::cout << str << std::endl;
    }
}

void Deck::addCard(const Card& card) {
    deck.push_back(card);
}

void Deck::drawCard(Hand& hand) {
    if (deck.empty())
        return;

    hand.addCard(deck.back());
    deck.pop_back();
}

void Deck::shuffleDeck() {
    auto rng = std::default_random_engine{std::random_device{}()};
    std::shuffle(deck.begin(), deck.end(), rng);
}
