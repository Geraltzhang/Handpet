#include "HealthScreen.h"

namespace V20 {
void HealthScreen::draw(VPetLCD* lcd) {
    // Title
    lcd->drawCharArrayOnLCD((char*)"HP", getPosX(), getPosY(), getPixelColor());
    // Bar
    int16_t barX = getPosX();
    int16_t barY = getPosY() + 8;
    int16_t barLen = 32;
    uint16_t hp = digimon->getHp();
    int16_t filled = (barLen * hp) / 100;
    for (int i=0; i<barLen; ++i) {
        bool on = i < filled;
        if (on) lcd->drawPixelOnLCD(barX + i, barY, getPixelColor());
    }
    // Sickness flag
    if (digimon->getIsSick()) {
        lcd->drawCharArrayOnLCD((char*)"SICK", barX, barY+4, getPixelColor());
    }
}
}


