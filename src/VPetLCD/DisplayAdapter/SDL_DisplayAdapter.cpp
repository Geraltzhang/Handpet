#ifndef ARDUINO
#include "SDL_DisplayAdapter.h"
#include <stdexcept>

static uint32_t rgb565_to_argb8888(uint16_t c, uint8_t a = 0xFF) {
    uint8_t r = ((c >> 11) & 0x1F) << 3;
    uint8_t g = ((c >> 5) & 0x3F) << 2;
    uint8_t b = (c & 0x1F) << 3;
    return (a << 24) | (r << 16) | (g << 8) | b;
}

SDL_DisplayAdapter::SDL_DisplayAdapter(uint16_t resolutionX, uint16_t resolutionY)
    : AbstractDisplayAdapter(resolutionX, resolutionY) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error("SDL_Init failed");
    }
    window = SDL_CreateWindow("VPet LCD", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              canvasWidth, canvasHeight, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    surface = SDL_CreateRGBSurfaceWithFormat(0, canvasWidth, canvasHeight, 32, SDL_PIXELFORMAT_ARGB8888);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, canvasWidth, canvasHeight);
}

SDL_DisplayAdapter::~SDL_DisplayAdapter() {
    if (surface) SDL_FreeSurface(surface);
    if (texture) SDL_DestroyTexture(texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

void SDL_DisplayAdapter::fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color) {
    SDL_Rect r{ x, y, w, h };
    uint32_t col = rgb565_to_argb8888((uint16_t)color);
    SDL_FillRect(surface, &r, col);
}

void SDL_DisplayAdapter::drawHLine(int32_t x, int32_t y, int32_t w, uint32_t color) {
    fillRect(x, y, w, 1, color);
}

void SDL_DisplayAdapter::drawVLine(int32_t x, int32_t y, int32_t h, uint32_t color) {
    fillRect(x, y, 1, h, color);
}

void SDL_DisplayAdapter::createCanvas() {
    // Surface already created in ctor
}

void SDL_DisplayAdapter::setColorDepth(int8_t b) {
    (void)b;
}

void SDL_DisplayAdapter::fillCanvas(uint32_t color) {
    SDL_FillRect(surface, nullptr, rgb565_to_argb8888((uint16_t)color));
}

void SDL_DisplayAdapter::pushCanvas(int32_t x, int32_t y, uint16_t transparent_color) {
    (void)x; (void)y; (void)transparent_color;
    SDL_UpdateTexture(texture, nullptr, surface->pixels, surface->pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void SDL_DisplayAdapter::deleteCanvas() {
    // keep surface for the lifetime
}
#endif


