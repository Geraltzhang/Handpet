#ifndef ARDUINO
#include "VPetLCD/VPetLCD.h"
#include "VPetLCD/VPetLCDMenuBar32p.h"
#include "VPetLCD/Screens/AgeWeightScreen.h"
#include "VPetLCD/Screens/DigimonNameScreen.h"
#include "VPetLCD/Screens/HeartsScreen.h"
#include "VPetLCD/Screens/ProgressBarScreen.h"
#include "VPetLCD/Screens/PercentageScreen.h"
#include "VPetLCD/Screens/SelectionScreen.h"
#include "VPetLCD/Screens/ClockScreen.h"
#include "VPetLCD/Screens/DigimonWatchingScreen.h"
#include "VPetLCD/Screens/AnimationScreens/EatingAnimationScreen.h"
#include "VPetLCD/Screens/PetSelectScreen.h"
#include "VPetLCD/Screens/TrainingScreen.h"
#include "VPetLCD/Screens/HealthScreen.h"
#include "VPetLCD/Screens/SettingsScreen.h"
#include "GameLogic/ScreenStateMachine.h"
#include "GameLogic/Digimon.h"

#include "VPetLCD/DisplayAdapter/SDL_DisplayAdapter.h"
#include "VPetLCD/LinuxSpriteManager.h"
#include "compat/ArduinoCompat.h"
#include <SDL2/SDL.h>
#include <chrono>

int main() {
    uint16_t digiIndex = DIGIMON_BOTAMON;
    Digimon digimon(digiIndex);
    const DigimonProperties* properties = &DIGIMON_DATA[digiIndex];
    digimon.setProperties(properties);

    int displayHeight = 360;
    int displayWidth = 640;
    SDL_DisplayAdapter displayAdapter(displayWidth, displayHeight);
    LinuxSpriteManager spriteManager("assets");

    VPetLCD screen(&displayAdapter, &spriteManager, 40, 16);
    VPetLCDMenuBar32p menuBar(7,5,displayHeight);

    V20::DigimonWatchingScreen digimonScreen(&spriteManager, digimon.getDigimonIndex(), -8, 40, 0, 0);
    PetSelectScreen petSelect(&spriteManager);
    V20::DigimonNameScreen digiNameScreen(&spriteManager, (char*)"BOTAMON", digimon.getDigimonIndex(), 24);
    V20::AgeWeightScreen ageWeightScreen(5, 21);
    V20::HeartsScreen hungryScreen("Hungry", 2, 4);
    V20::HeartsScreen strengthScreen("Strength", 3, 4);
    V20::HeartsScreen effortScreen("Effort", 4, 4);
    V20::ProgressBarScreen dpScreen("DP", 29, 40);
    V20::PercentageScreen sPercentageScreen("WIN", 'S', 100);
    V20::PercentageScreen tPercentageScreen("WIN", 'T', 93);
    V20::SelectionScreen foodSelection(true);
    V20::SelectionScreen fightSelection(true);
    V20::ClockScreen clockScreen(true);
    V20::EatingAnimationScreen eatingAnimationScreen(&spriteManager, digimon.getDigimonIndex());
    V20::TrainingScreen trainingScreen(&digimon);
    V20::HealthScreen healthScreen(&digimon);
    SettingsScreen settingsScreen;

    uint8_t numberOfScreens = 17;
    uint8_t numberOfSignals = 3;
    uint8_t confirmSignal = 0;
    uint8_t nextSignal = 1;
    uint8_t backSignal = 2;

    ScreenStateMachine stateMachine(numberOfScreens, numberOfSignals);
    uint8_t digimonScreenId = stateMachine.addScreen(&digimonScreen);
    uint8_t petSelectId = stateMachine.addScreen(&petSelect);
    uint8_t digiNameScreenId = stateMachine.addScreen(&digiNameScreen);
    uint8_t ageWeightScreenId = stateMachine.addScreen(&ageWeightScreen);
    uint8_t hungryScreenId = stateMachine.addScreen(&hungryScreen);
    uint8_t strengthScreenId = stateMachine.addScreen(&strengthScreen);
    uint8_t effortScreenId = stateMachine.addScreen(&effortScreen);
    uint8_t dpScreenId = stateMachine.addScreen(&dpScreen);
    uint8_t sPercentageScreenId = stateMachine.addScreen(&sPercentageScreen);
    uint8_t tPercentageScreenId = stateMachine.addScreen(&tPercentageScreen);
    uint8_t foodSelectionId = stateMachine.addScreen(&foodSelection);
    uint8_t fightSelectionId = stateMachine.addScreen(&fightSelection);
    uint8_t clockScreenId = stateMachine.addScreen(&clockScreen);
    uint8_t eatingAnimationScreenId = stateMachine.addScreen(&eatingAnimationScreen);
    uint8_t trainingScreenId = stateMachine.addScreen(&trainingScreen);
    uint8_t healthScreenId = stateMachine.addScreen(&healthScreen);
    uint8_t settingsScreenId = stateMachine.addScreen(&settingsScreen);

    eatingAnimationScreen.setAnimationEndAction([&]() { stateMachine.setCurrentScreen(foodSelectionId); });

    for (int i = 1; i <= numberOfScreens + 1;i++) { stateMachine.addTransition(i, digimonScreenId, backSignal); }
    stateMachine.addTransition(digiNameScreenId, ageWeightScreenId, nextSignal);
    stateMachine.addTransition(ageWeightScreenId, hungryScreenId, nextSignal);
    stateMachine.addTransition(hungryScreenId, strengthScreenId, nextSignal);
    stateMachine.addTransition(strengthScreenId, effortScreenId, nextSignal);
    stateMachine.addTransition(effortScreenId, dpScreenId, nextSignal);
    stateMachine.addTransition(dpScreenId, sPercentageScreenId, nextSignal);
    stateMachine.addTransition(sPercentageScreenId, tPercentageScreenId, nextSignal);
    stateMachine.addTransition(tPercentageScreenId, trainingScreenId, nextSignal);
    stateMachine.addTransition(trainingScreenId, healthScreenId, nextSignal);
    stateMachine.addTransition(healthScreenId, digiNameScreenId, nextSignal);
    // open settings from clock screen via confirm
    stateMachine.addTransition(clockScreenId, settingsScreenId, confirmSignal);
    stateMachine.addTransition(settingsScreenId, clockScreenId, backSignal);
    stateMachine.addTransition(digimonScreenId, clockScreenId, backSignal);
    stateMachine.addTransition(digimonScreenId, petSelectId, confirmSignal);
    stateMachine.addTransition(petSelectId, digimonScreenId, confirmSignal);
    stateMachine.addTransitionAction(petSelectId, confirmSignal, [&]() {
        uint16_t idx = (uint16_t)petSelect.getSelection();
        digimon.setDigimonIndex(idx);
    });
    stateMachine.addTransitionAction(petSelectId, nextSignal, [&]() { petSelect.next(); });
    stateMachine.addTransitionAction(petSelectId, backSignal, [&]() { petSelect.prev(); });
    stateMachine.addTransition(clockScreenId, digimonScreenId, backSignal);
    stateMachine.addTransition(digimonScreenId, digimonScreenId, nextSignal);
    stateMachine.addTransitionAction(digimonScreenId, nextSignal, [&]() { menuBar.nextSelection(); });
    stateMachine.addTransition(digimonScreenId, digimonScreenId, confirmSignal);
    stateMachine.addTransitionAction(digimonScreenId, confirmSignal, [&]() {
        uint8_t maxdp = digimon.getProperties()->maxDigimonPower;
        switch (menuBar.getSelection()) {
            case 0:
                digiNameScreen.setDigimonSpriteIndex(digimon.getDigimonIndex());
                hungryScreen.setHearts(4-4*digimon.getHunger()/10);
                if(maxdp >0){ dpScreen.setFillPercentage((digimon.getDigimonPower()*100)/maxdp);} else { dpScreen.setFillPercentage(0);} 
                ageWeightScreen.setAge(digimon.getAge());
                ageWeightScreen.setWeight(digimon.getWeight());
                stateMachine.setCurrentScreen(digiNameScreenId);
                break;
            case 1:
                foodSelection.setSelection(0);
                stateMachine.setCurrentScreen(foodSelectionId);
                break;
            case 3:
                fightSelection.setSelection(0);
                stateMachine.setCurrentScreen(fightSelectionId);
                break;
            case 4:
                digimonScreen.flushPoop();
                digimon.setNumberOfPoops(0);
                break;
            case 5:
                stateMachine.setCurrentScreen(trainingScreenId);
                break;
            case 2:
                break;
        }
    });
    stateMachine.addTransition(foodSelectionId, foodSelectionId, nextSignal);
    stateMachine.addTransitionAction(foodSelectionId, nextSignal, [&]() { foodSelection.nextSelection(); });
    stateMachine.addTransition(foodSelectionId, foodSelectionId, confirmSignal);
    stateMachine.addTransitionAction(foodSelectionId, confirmSignal, [&]() {
        uint8_t selection = foodSelection.getSelection();
        switch (selection) {
            case 0:
                if (digimon.canFeed()) {
                    digimon.addWeight(1);
                    digimon.reduceHunger(1);
                    eatingAnimationScreen.setSprites(SYMBOL_MEAT, SYMBOL_HALF_MEAT,SYMBOL_EMPTY_MEAT);
                    eatingAnimationScreen.startAnimation();
                    stateMachine.setCurrentScreen(eatingAnimationScreenId);
                    digimon.setFeedCooldown(1000UL*30); // 30s cooldown
                    // overfeed penalty when hunger已满
                    if (digimon.getHunger() >= 10) { digimon.addHp(-2); }
                }
                break;
            case 1:
                // pill: cure sickness and small DP gain, limited by cooldown
                if (digimon.canFeed()) {
                    digimon.addWeight(2);
                    digimon.addStrength(2);
                    digimon.addDigimonPower(2);
                    digimon.cure();
                    eatingAnimationScreen.setSprites(SYMBOL_PILL, SYMBOL_HALF_PILL,SYMBOL_EMPTY);
                    eatingAnimationScreen.startAnimation();
                    stateMachine.setCurrentScreen(eatingAnimationScreenId);
                    digimon.setFeedCooldown(1000UL*60); // 60s cooldown for medicine
                }
                break;
            case 2:
                eatingAnimationScreen.setSprites(SYMBOL_HEART, SYMBOL_HEARTEMPTY,SYMBOL_EMPTY);
                eatingAnimationScreen.startAnimation();
                stateMachine.setCurrentScreen(eatingAnimationScreenId);
                break;
            case 3:
                if (digimon.canClean()) {
                    eatingAnimationScreen.setSprites(SYMBOL_POOP,SYMBOL_HALF_PILL,SYMBOL_EMPTY);
                    eatingAnimationScreen.startAnimation();
                    stateMachine.setCurrentScreen(eatingAnimationScreenId);
                    digimon.cleanPoop();
                    digimon.setCleanCooldown(1000UL*20); // 20s cooldown
                }
                break;
        }
    });
    stateMachine.addTransition(eatingAnimationScreenId, foodSelectionId, confirmSignal);
    stateMachine.addTransitionAction(eatingAnimationScreenId, confirmSignal, [&](){ eatingAnimationScreen.abortAnimation(); });

    // training controls
    stateMachine.addTransition(trainingScreenId, trainingScreenId, nextSignal);
    stateMachine.addTransitionAction(trainingScreenId, nextSignal, [&](){ trainingScreen.harder(); });
    stateMachine.addTransition(trainingScreenId, trainingScreenId, backSignal);
    stateMachine.addTransitionAction(trainingScreenId, backSignal, [&](){ trainingScreen.easier(); });
    stateMachine.addTransition(trainingScreenId, trainingScreenId, confirmSignal);
    stateMachine.addTransitionAction(trainingScreenId, confirmSignal, [&](){ if(digimon.canTrain()){ trainingScreen.confirm(); digimon.setTrainCooldown(1000UL*45);} });

    menuBar.setIconOnIndex(0,0);
    menuBar.setIconOnIndex(1,1);
    menuBar.setIconOnIndex(2,2);
    menuBar.setIconOnIndex(3,3);
    menuBar.setIconOnIndex(4,4);
    menuBar.setIconOnIndex(5,5);
    menuBar.setIconOnIndex(6,6);
    screen.setMenuBar(&menuBar);
    screen.setLCDPos(0, 32);
    screen.setLcdScale(8);

    int screensOffsetX = 4;
    ageWeightScreen.setPos(screensOffsetX, 0);
    effortScreen.setPos(screensOffsetX, 0);
    strengthScreen.setPos(screensOffsetX, 0);
    hungryScreen.setPos(screensOffsetX, 0);
    dpScreen.setPos(screensOffsetX, 0);
    sPercentageScreen.setPos(screensOffsetX, 0);
    tPercentageScreen.setPos(screensOffsetX, 0);
    clockScreen.setPos(screensOffsetX, 0);
    eatingAnimationScreen.setPos(screensOffsetX, 0);

    foodSelection.addOption("Meat", SYMBOL_MEAT);
    foodSelection.addOption("PILL", SYMBOL_PILL);
    foodSelection.addOption("LOVE", SYMBOL_HEART);
    foodSelection.addOption("SHIT", SYMBOL_POOP);
    fightSelection.setShowIcons(false);
    fightSelection.addOption("SINGLE");
    fightSelection.addOption("TAG");

    bool running = true;
    auto last = std::chrono::steady_clock::now();
    while (running) {
        SDL_Event e; 
        while (SDL_PollEvent(&e)) { 
            if (e.type == SDL_QUIT) running=false; 
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE: running=false; break;
                    case SDLK_RIGHT: stateMachine.sendSignal(nextSignal); break;
                    case SDLK_RETURN: case SDLK_SPACE: stateMachine.sendSignal(confirmSignal); break;
                    case SDLK_LEFT: case SDLK_BACKSPACE: stateMachine.sendSignal(backSignal); break;
                }
            }
        }

        auto now = std::chrono::steady_clock::now();
        unsigned long delta = (unsigned long)std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();
        last = now;

        digimon.loop(delta);
        digimonScreen.setNumberOfPoop(digimon.getNumberOfPoops());
        digimonScreen.loop(delta);
        clockScreen.loop(delta);
        digiNameScreen.loop(delta);
        // Sleep schedule: 22:00-08:00 sleep
        bool sleeping = (clockScreenId, true);
        (void)sleeping; // silence unused if compiled without logic below
        // simple time-based flag using system clock; we'll reuse SDL for time but here we cannot fetch real time; keep placeholder
        // set watching screen sleeping flag from game state if needed
        if (stateMachine.getCurrentScreen() == &eatingAnimationScreen) eatingAnimationScreen.loop(delta);
        screen.renderScreen(stateMachine.getCurrentScreen());
        SDL_Delay(16);
    }
    return 0;
}
#endif


