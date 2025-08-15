#pragma once
#ifndef ARDUINO
#include "AbstractSpriteManager.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <librsvg/rsvg.h>
#include <cairo/cairo.h>
#include "ImageData/LettersAndDigits.h"
#include "ImageData/SymbolData.h"
#include "ImageData/HighResMenuItems.h"
#include "../compat/ArduinoCompat.h"

struct SvgFrameKey {
    uint16_t digimonIndex;
    uint16_t frameIndex;
    bool operator==(const SvgFrameKey &o) const { return digimonIndex==o.digimonIndex && frameIndex==o.frameIndex; }
};

namespace std { template<> struct hash<SvgFrameKey>{ size_t operator()(SvgFrameKey const& k) const noexcept { return ((size_t)k.digimonIndex<<16) ^ k.frameIndex; } }; }

class LinuxSpriteManager : public AbstractSpriteManager {
private:
    std::string assetDir;
    std::unordered_map<SvgFrameKey, std::vector<uint16_t>> svgCache;
    // For non-digimon assets we reuse built-in bitmaps

    std::vector<uint16_t> renderSvgToMonoGrid(const std::string& path, int w, int h);
    std::vector<byte> renderSvgToMonoBytes(const std::string& path, int w, int h);
    std::vector<uint32_t> renderSvgToRowBits(const std::string& path, int w, int h);

public:
    LinuxSpriteManager(const std::string& _assetDir);
    ~LinuxSpriteManager();
    const std::string& getAssetDir() const { return assetDir; }

    const unsigned short* getDigimonSprite(uint16_t digimonIndex, uint16_t spriteIndex) override;
    const byte* getSmallDigitsSprite(uint8_t index) override;
    const byte* getDigitSprite(uint8_t digit) override;
    const byte* getSmallCapitalLetter(char c) override;
    const uint8_t getSmallCapitalLetterWidth(char c) override;
    const byte* getSymbol(uint16_t symbolIndex) override;
    const uint32_t* getHighResMenuItem(uint16_t menuItemIndex) override;
};
#endif


