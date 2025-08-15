#pragma once
#include <string>
#include "../GameLogic/Digimon.h"

namespace Save {
struct State {
    uint16_t index;
    uint16_t age;
    uint16_t weight;
    uint8_t hunger;
    uint8_t strength;
    uint8_t effort;
    uint8_t dp;
    uint8_t hp;
    uint8_t poops;
    bool sick;
};

inline std::string path(){ return "save.json"; }
bool load(Digimon& d);
bool save(const Digimon& d);
}


