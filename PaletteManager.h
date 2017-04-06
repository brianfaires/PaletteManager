#ifndef PALETTE_MANAGER_H
#define PALETTE_MANAGER_H

#include "Arduino.h"
#include "FastLED.h"

#define MAX_BRIGHTNESS 100
#define INTERVAL_PALETTE_SWITCHING 7

#define NUM_PALETTES 10
#define PN_OCEAN 0
#define PN_LAVA 1
#define PN_FOREST 2
#define PN_CLOUD 3
#define PN_PARTY 4
#define PN_RAINBOW 5
#define PN_HEAT 6
#define PN_PASTEL 7
#define PN_LOVE 8
#define PN_STROBE 9

#define NUM_WALK_STYLES 10
#define WS_NONE 0
#define WS_SWITCH_SLOW 1
#define WS_SWITCH_FAST 2
#define WS_BLEND_SLOW 3
#define WS_BLEND_FAST 4
#define WS_RAND_SLOW 5
#define WS_RAND_FAST 6
#define WS_CYCLE_SLOW 7
#define WS_CYCLE_MED 8
#define WS_CYCLE_FAST 9

#define COMP_BITMASK_R   1 << 6
#define COMP_BITMASK_G   1 << 5
#define COMP_BITMASK_B   1 << 4
#define COMP_BITMASK_RG  1 << 3
#define COMP_BITMASK_RB  1 << 2
#define COMP_BITMASK_GB  1 << 1
#define COMP_BITMASK_RGB 1 << 0

class PaletteManager {
  public:
    CRGBPalette16 palette;
    uint16_t updateInterval;

    PaletteManager();
    void NextPalette();
    void NextWalkStyle();
    void Update();
    
    CHSV GetRandomColor(uint8_t brightness = 0);
    
  private:
    void PaletteRGB2HSV(CRGBPalette16* rgbPalette, CHSVPalette16* hsvPalette);
    void SetUpdateInterval();
    void StartPaletteTransition();
    CRGBPalette16 GetPalette(uint8_t paletteName);
    
    uint8_t curPalette;
    uint8_t walkStyle;
    bool switchingPalette;
    CHSVPalette16 palette_hsv;
    CRGBPalette16 targetPalette;
};
#endif
