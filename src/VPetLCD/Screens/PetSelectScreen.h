#pragma once
#ifndef ARDUINO
#include "../VPetLCD.h"
#include "../LinuxSpriteManager.h"
#include <vector>

class PetSelectScreen : public VPetLCD::Screen {
private:
    LinuxSpriteManager* linuxSpriteManager;
    int currentSelection;
public:
    PetSelectScreen(LinuxSpriteManager* sm);
    void next();
    void prev();
    int getSelection() const { return currentSelection; }
    void draw(VPetLCD* lcd) override;
};
#endif


