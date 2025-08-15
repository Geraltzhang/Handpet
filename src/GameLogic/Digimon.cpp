#include "Digimon.h"

void Digimon::printSerial(){

  Serial.println(getDigimonIndex());
  Serial.println(getState());
  Serial.println(getAge());
  Serial.println(getWeight());
  Serial.println(getFeedCounter());
  Serial.println(getCareMistakes());
  Serial.println(getTrainingCounter());
  Serial.println(getPoopTimer());
  Serial.println(getAgeTimer());
  Serial.println(getEvolutionTimer());
}


void Digimon::loop(unsigned long delta){

    updateTimers(delta);
    tickCooldowns(delta);
}


void Digimon::updateTimers(unsigned long delta){
    poopTimer += delta;
    if(poopTimer > properties->poopTimeSec*1000){
        poopTimer %= properties->poopTimeSec*1000;
        numberOfPoops++;
        loseWeight(1);
    }

    ageTimer += delta;
    uint32_t day = 1000*60*60*24;
    if(ageTimer >= day){
        ageTimer %= day;
        age++;
    }

    evolutionTimer += delta;
    if(evolutionTimer >= properties->evolutionTimeSec*1000){
        evolutionTimer =0;
        evolved = true;
    }

    // hunger decay every 5 minutes
    hungerTimerMs += delta;
    if (hungerTimerMs >= 1000UL*60*5) {
        hungerTimerMs %= 1000UL*60*5;
        if (hunger>0) {
            hunger--;
        } else {
            addHp(-1);
            sicknessCounter++;
            if (sicknessCounter > 10) isSick = true;
        }
    }

    // health adjustment every 30s
    healthTimerMs += delta;
    if (healthTimerMs >= 1000UL*30) {
        healthTimerMs %= 1000UL*30;
        if (!isSick) addHp(+1); else addHp(-1);
    }

    // periodic sickness roll influenced by cleanliness (numberOfPoops)
    sicknessRollTimerMs += delta;
    if (sicknessRollTimerMs >= 1000UL*60) { // every minute
        sicknessRollTimerMs %= 1000UL*60;
        // base chance
        int chance = 1 + numberOfPoops*5; // more poop -> higher chance
        if (hunger==0) chance += 5;       // starving increases chance
        if ((rand() % 100) < chance) {
            isSick = true;
        }
    }
}