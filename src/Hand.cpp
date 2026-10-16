
#include "Hand.hpp"

void Hand::addCard(Card* card) {
    if (card)
        cards.push_back(card);
}

void Hand::removeCard(int index) {
    if (index < 0 || index >= size())
        return;
    cards.erase(cards.begin() + index);
}

Card* Hand::getCard(int index) const {
    if (index < 0 || index >= size())
        return nullptr;

    return cards[index];
}