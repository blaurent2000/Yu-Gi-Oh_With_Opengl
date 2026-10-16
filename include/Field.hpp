
#pragma once
#include "FieldZone.hpp"
#include <vector>

class Field {
    public:
        Field();

        const std::vector<FieldZone>& getZones() const {return zones;}
        FieldZone* getZone(int index);

        bool placeCard(int index, Card* card);
        void removeCard(int index);
    private:
        std::vector<FieldZone> zones;
};