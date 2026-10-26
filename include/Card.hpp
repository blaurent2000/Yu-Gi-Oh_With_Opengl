
#pragma once

#include <string>

enum CardType {
    MONSTER,
    SPELL,
    TRAP,
    FIELD
};

class Card {
public:
    Card(const std::string& name, CardType type, int attack, int defense,
         int level = 0, const std::string& description = "");

    const std::string& getName() const;
    CardType getType() const;
    int getAttack() const;
    void setAttack(int a);
    int getDefense() const;
    void setDefense(int d);
    int getLvl() const;
    void setLvl(int l);
    const std::string& getDescription() const;
    bool isFaceDown() const;
    void setFaceDown(bool faceDown);
    void flip();

private:
    std::string name;
    CardType type;
    int attack;
    int defense;
    int lvl;
    std::string description;
    bool faceDown;
};