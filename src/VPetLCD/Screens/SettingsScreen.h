#pragma once
#include "../VPetLCD.h"
#include "../Screens/SelectionScreen.h"
#include "../../GameLogic/Digimon.h"

class SettingsScreen : public VPetLCD::Screen {
private:
    V20::SelectionScreen menu;
    uint8_t speedMultiplier = 1; // 1x,2x,3x
public:
    SettingsScreen(): menu(true){
        menu.setShowText(true);
        menu.addOption((char*)"TIME");
        menu.addOption((char*)"VOLUME");
        menu.addOption((char*)"SPEED");
        menu.addOption((char*)"THEME");
    }
    void next(){ menu.nextSelection(); }
    void back(){ /* reserved */ }
    void confirm(){ /* placeholder: apply setting */ }
    void draw(VPetLCD* lcd) override { menu.setPos(getPosX(), getPosY()); menu.setPixelColor(getPixelColor()); menu.draw(lcd); }
};


