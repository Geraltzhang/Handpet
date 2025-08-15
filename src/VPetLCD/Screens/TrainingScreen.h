#pragma once
#include "../VPetLCD.h"
#include "../../GameLogic/Digimon.h"

namespace V20 {
class TrainingScreen : public VPetLCD::Screen {
private:
    Digimon* digimon;
    uint8_t cursor = 0; // 0..2 levels
public:
    explicit TrainingScreen(Digimon* d) : digimon(d) {}
    void harder(){ if(cursor<2) cursor++; }
    void easier(){ if(cursor>0) cursor--; }
    void confirm(){
        // simple effect: higher level grants more DP and effort, consumes HP
        uint8_t lvl = cursor;
        digimon->addDigimonPower(1 + lvl);
        digimon->setEffort(digimon->getEffort() + (1 + lvl));
        digimon->addHp(-(int8_t)(2 + 2*lvl));
    }
    void draw(VPetLCD* lcd) override;
};
}


