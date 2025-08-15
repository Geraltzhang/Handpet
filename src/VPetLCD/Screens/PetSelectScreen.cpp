#ifndef ARDUINO
#include "PetSelectScreen.h"
#include <filesystem>
#include <regex>

using std::string;
namespace fs = std::filesystem;

static bool isNumber(const string& s){ return std::regex_match(s, std::regex("[0-9]+")); }

PetSelectScreen::PetSelectScreen(LinuxSpriteManager* sm) : linuxSpriteManager(sm) {
    currentSelection = 0;
}

void PetSelectScreen::next(){ currentSelection++; }
void PetSelectScreen::prev(){ if(currentSelection>0) currentSelection--; }

void PetSelectScreen::draw(VPetLCD* lcd) {
    // Draw simple title using built-in letters
    lcd->drawCharArrayOnLCD((char*)"SELECT", 2, 0, getPixelColor());

    // Try to render thumbnail: assets/digimon/<id>/thumb.svg at 16x16 placed center
    int centerX = 12;
    int centerY = 0;
    const unsigned short* sprite = linuxSpriteManager->getDigimonSprite((uint16_t)currentSelection, 0);
    lcd->draw16BitArray(sprite, screenX + centerX, screenY + centerY, false, getPixelColor());

    // Show index number
    lcd->drawIntegerOnLCD(currentSelection, screenX + 34, screenY + 12, getPixelColor());
}
#endif


