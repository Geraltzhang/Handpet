/////////////////////////////////////////////////////////////////
/*
  Created by Berat Özdemir, January 24 , 2021.
*/
/////////////////////////////////////////////////////////////////

#include "../VPetLCD.h"

namespace V20 {
  /**
 *
 * Renders a Screen showing a screen with a text above a percentage infomation.
 * (For example Displaying WIN Rate)
 *
 * */
  class ClockScreen : public VPetLCD::Screen {
  private:
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    boolean showAMPM;
    boolean isSetMode;
    boolean sleeping = false;
    
    void incrementSeconds();

  public:
    ClockScreen(boolean _showAMPM);
    void loop(unsigned long delta);
    void setHours(uint8_t _hours) { hours = _hours; }
    void setMinutes(uint8_t _minutes) { minutes = _minutes; }
    void setSeconds(uint8_t _seconds) { seconds = _seconds; }
    void setSleeping(boolean s){ sleeping=s; }
    uint8_t getHours() const { return hours; }
    uint8_t getMinutes() const { return minutes; }
    uint8_t getSeconds() const { return seconds; }

 

    void draw(VPetLCD* lcd);

  };

}