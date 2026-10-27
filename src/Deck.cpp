
#include "Deck.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(" \t\r\n");

    if (first == std::string::npos)
        return "";

    size_t last = str.find_last_not_of(" \t\r\n");

    return str.substr(first, last - first + 1);
}

CardType stringToCardType(const std::string& type) {
    if (type == "MONSTER")
        return CardType::MONSTER;
    if (type == "SPELL")
        return CardType::SPELL;
    if (type == "TRAP")
        return CardType::TRAP;
    if (type == "FIELD")
        return CardType::FIELD;

    throw std::invalid_argument("Type de carte inconnu : " + type);
}

void Deck::initDeck(const std::string& path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cerr << "Impossible d'ouvrir le fichier : "
                  << path << std::endl;
        return;
    }

    std::string line;

    while (std::getline(file, line))
    {
        line = trim(line);

        if (line.empty() || line[0] == '#')
            continue;

        std::stringstream ss(line);
        std::string field;

        std::string name;
        CardType type;
        int attack;
        int defense;
        int level;

        if (!std::getline(ss, field, '|'))
            continue;
        name = trim(field);

        if (!std::getline(ss, field, '|'))
            continue;
        type = stringToCardType(trim(field));

        if (!std::getline(ss, field, '|'))
            continue;
        attack = std::stoi(trim(field));

        if (!std::getline(ss, field, '|'))
            continue;
        defense = std::stoi(trim(field));

        if (!std::getline(ss, field, '|'))
            continue;
        level = std::stoi(trim(field));

        Card cardToAdd(name, type, attack, defense, level);
        addCard(cardToAdd);
    }
    shuffleDeck();
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

int Deck::getDeckSize() const {
    return deck.size();
}