#pragma once
#ifndef ARDUINO
#include "AbstractDisplayadapter.h"
#include <SDL2/SDL.h>

class SDL_DisplayAdapter : public AbstractDisplayAdapter {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Surface* surface;

public:
    SDL_DisplayAdapter(uint16_t resolutionX, uint16_t resolutionY);
    ~SDL_DisplayAdapter();

    void fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color) override;
    void drawHLine(int32_t x, int32_t y, int32_t w, uint32_t color) override;
    void drawVLine(int32_t x, int32_t y, int32_t h, uint32_t color) override;
    void createCanvas() override;
    void setColorDepth(int8_t b) override;
    void fillCanvas(uint32_t color) override;
    void pushCanvas(int32_t x, int32_t y, uint16_t transparent_color) override;
    void deleteCanvas() override;
};
#endif


