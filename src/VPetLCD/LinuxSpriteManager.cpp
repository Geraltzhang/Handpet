#ifndef ARDUINO
#include "LinuxSpriteManager.h"
#include <sstream>
#include <stdexcept>
#include <fstream>

LinuxSpriteManager::LinuxSpriteManager(const std::string& _assetDir) : assetDir(_assetDir) {}

LinuxSpriteManager::~LinuxSpriteManager() {}

std::vector<uint16_t> LinuxSpriteManager::renderSvgToMonoGrid(const std::string& path, int w, int h) {
    std::vector<uint16_t> rows(h, 0);
    GError* err = nullptr;
    RsvgHandle* handle = rsvg_handle_new_from_file(path.c_str(), &err);
    if (!handle) throw std::runtime_error("Failed to load SVG: "+path);

    cairo_surface_t* surf = cairo_image_surface_create(CAIRO_FORMAT_A8, w, h);
    cairo_t* cr = cairo_create(surf);
    cairo_scale(cr, (double)w, (double)h);
    rsvg_handle_render_cairo(handle, cr);

    cairo_surface_flush(surf);
    unsigned char* data = cairo_image_surface_get_data(surf);
    int stride = cairo_image_surface_get_stride(surf);
    for (int y=0; y<h; ++y) {
        uint16_t mask = 0x8000;
        for (int x=0; x<w; ++x) {
            unsigned char a = data[y*stride + x];
            if (a > 127) rows[y] |= mask;
            mask >>= 1;
        }
    }
    cairo_destroy(cr);
    cairo_surface_destroy(surf);
    g_object_unref(handle);
    return rows;
}

std::vector<byte> LinuxSpriteManager::renderSvgToMonoBytes(const std::string& path, int w, int h) {
    std::vector<byte> rows(h, 0);
    GError* err = nullptr;
    RsvgHandle* handle = rsvg_handle_new_from_file(path.c_str(), &err);
    if (!handle) throw std::runtime_error("Failed to load SVG: "+path);

    cairo_surface_t* surf = cairo_image_surface_create(CAIRO_FORMAT_A8, w, h);
    cairo_t* cr = cairo_create(surf);
    cairo_scale(cr, (double)w, (double)h);
    rsvg_handle_render_cairo(handle, cr);

    cairo_surface_flush(surf);
    unsigned char* data = cairo_image_surface_get_data(surf);
    int stride = cairo_image_surface_get_stride(surf);
    for (int y=0; y<h; ++y) {
        byte mask = 0x80;
        for (int x=0; x<w; ++x) {
            unsigned char a = data[y*stride + x];
            if (a > 127) rows[y] |= mask;
            mask >>= 1;
        }
    }
    cairo_destroy(cr);
    cairo_surface_destroy(surf);
    g_object_unref(handle);
    return rows;
}

std::vector<uint32_t> LinuxSpriteManager::renderSvgToRowBits(const std::string& path, int w, int h) {
    std::vector<uint32_t> rows(h, 0);
    GError* err = nullptr;
    RsvgHandle* handle = rsvg_handle_new_from_file(path.c_str(), &err);
    if (!handle) throw std::runtime_error("Failed to load SVG: "+path);
    cairo_surface_t* surf = cairo_image_surface_create(CAIRO_FORMAT_A8, w, h);
    cairo_t* cr = cairo_create(surf);
    cairo_scale(cr, (double)w, (double)h);
    rsvg_handle_render_cairo(handle, cr);
    cairo_surface_flush(surf);
    unsigned char* data = cairo_image_surface_get_data(surf);
    int stride = cairo_image_surface_get_stride(surf);
    for (int y=0; y<h; ++y) {
        uint32_t mask = 0x80000000u;
        for (int x=0; x<w; ++x) {
            unsigned char a = data[y*stride + x];
            if (a > 127) rows[y] |= mask;
            mask >>= 1;
        }
    }
    cairo_destroy(cr);
    cairo_surface_destroy(surf);
    g_object_unref(handle);
    return rows;
}

const unsigned short* LinuxSpriteManager::getDigimonSprite(uint16_t digimonIndex, uint16_t spriteIndex) {
    SvgFrameKey key{digimonIndex, spriteIndex};
    auto it = svgCache.find(key);
    if (it == svgCache.end()) {
        std::ostringstream oss; oss << assetDir << "/digimon/" << digimonIndex << "/" << spriteIndex << ".svg";
        std::ifstream f(oss.str());
        if (f.good()) {
            std::vector<uint16_t> rows = renderSvgToMonoGrid(oss.str(), 16, 16);
            svgCache.emplace(key, rows);
        } else {
            // Fallback to built-in bitmap
            std::vector<uint16_t> rows(16, 0);
            const unsigned short* src = DIGIMON[digimonIndex][spriteIndex];
            for (int y=0; y<16; ++y) rows[y] = src[y];
            svgCache.emplace(key, rows);
        }
        it = svgCache.find(key);
    }
    return reinterpret_cast<const unsigned short*>(it->second.data());
}

const byte* LinuxSpriteManager::getSmallDigitsSprite(uint8_t index) {
    return SMALLDIGITS[index];
}

const byte* LinuxSpriteManager::getDigitSprite(uint8_t digit) {
    return DIGITS[digit];
}

const byte* LinuxSpriteManager::getSmallCapitalLetter(char c) {
    if (c > 'Z') c -= 32;
    auto it = letterCache.find(c);
    if (it == letterCache.end()) {
        // provide direct access to built-in bitmap
        return CAPITAL_ALPHABET_SMALL[c - 65];
    }
    return reinterpret_cast<const byte*>(it->second.data());
}

const uint8_t LinuxSpriteManager::getSmallCapitalLetterWidth(char c) {
    if (c > 'Z') c -= 32;
    int arrayPos = c - 65;
    return CAPITAL_LETTER_WIDTH[arrayPos];
}

const byte* LinuxSpriteManager::getSymbol(uint16_t symbolIndex) {
    return SYMBOLS[symbolIndex];
}

const uint32_t* LinuxSpriteManager::getHighResMenuItem(uint16_t menuItemIndex) {
    return MENU_ITEMS[menuItemIndex];
}
#endif


