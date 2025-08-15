#pragma once
#include "../VPetLCD.h"
#include "../../GameLogic/Digimon.h"

namespace V20 {
class HealthScreen : public VPetLCD::Screen {
private:
    Digimon* digimon;
public:
    explicit HealthScreen(Digimon* d) : digimon(d) {}
    void draw(VPetLCD* lcd) override;
};
}


