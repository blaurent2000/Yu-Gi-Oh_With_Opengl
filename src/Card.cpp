
#include "Card.hpp"

Card::Card(const std::string& name, CardType type, int attack, int defense,
           int level, const std::string& description)
    : name(name), type(type), attack(attack), defense(defense),
      lvl(level), description(description), faceDown(false) {}

const std::string& Card::getName() const { return name; }
CardType Card::getType() const { return type; }
int Card::getAttack() const { return attack; }
void Card::setAttack(int a) { attack = a; }
int Card::getDefense() const { return defense; }
void Card::setDefense(int d) { defense = d; }
int Card::getLvl() const { return lvl; }
void Card::setLvl(int l) { lvl = l; }
const std::string& Card::getDescription() const { return description; }
bool Card::isFaceDown() const { return faceDown; }
void Card::setFaceDown(bool faceDown) { this->faceDown = faceDown; }
void Card::flip() { faceDown = !faceDown; }
