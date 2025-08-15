#include "TrainingScreen.h"

namespace V20 {
void TrainingScreen::draw(VPetLCD* lcd) {
    lcd->drawCharArrayOnLCD((char*)"TRAIN", getPosX(), getPosY(), getPixelColor());
    int16_t x = getPosX();
    int16_t y = getPosY() + 8;
    for (int i=0; i<3; ++i){
        // draw three boxes, fill up to cursor
        for (int px=0; px<6; ++px) for (int py=0; py<6; ++py){
            if (i <= cursor) lcd->drawPixelOnLCD(x + i*8 + px, y + py, getPixelColor());
        }
    }
}
}


