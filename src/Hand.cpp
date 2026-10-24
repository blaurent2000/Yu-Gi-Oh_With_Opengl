
#include "Hand.hpp"

void Hand::addCard(const Card& card) {
    cards.push_back(card);
}

void Hand::removeCard(int index) {
    if (index < 0 || index >= size())
        return;

    cards.erase(cards.begin() + index);
}

Card* Hand::getCard(int index) {
    if (index < 0 || index >= size())
        return nullptr;

    return &cards[index];
}

const std::vector<Card>& Hand::getCards() const {
    return cards;
}

int Hand::size() const {
    return static_cast<int>(cards.size());
}
