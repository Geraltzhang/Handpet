#include "LinuxSave.h"
#include <fstream>

using namespace Save;

bool Save::load(Digimon& d){
    std::ifstream f(path()); if(!f.good()) return false;
    State s{}; f.read(reinterpret_cast<char*>(&s), sizeof(State)); if(!f) return false;
    d.setDigimonIndex(s.index);
    d.setAge(s.age);
    d.setWeight(s.weight);
    d.setHunger(s.hunger);
    d.setStrength(s.strength);
    d.setEffort(s.effort);
    d.setDigimonPower(s.dp);
    d.setHp(s.hp);
    d.setNumberOfPoops(s.poops);
    d.setSick(s.sick);
    return true;
}

bool Save::save(const Digimon& d){
    State s{};
    s.index = d.getDigimonIndex();
    s.age = d.getAge();
    s.weight = d.getWeight();
    s.hunger = d.getHunger();
    s.strength = d.getStrength();
    s.effort = d.getEffort();
    s.dp = d.getDigimonPower();
    s.hp = d.getHp();
    s.poops = d.getNumberOfPoops();
    s.sick = d.getIsSick();
    std::ofstream f(path(), std::ios::binary|std::ios::trunc);
    f.write(reinterpret_cast<const char*>(&s), sizeof(State));
    return (bool)f;
}


