#include "Arduino.h"
#include "PaletteManager.h"
#include "CustomPalettes.h"

PaletteManager::PaletteManager() {
  curPalette = 0;
  walkStyle = 0;
  updateInterval = 0;
  switchingPalette = false;
  palette = PastelColors_p;
  PaletteRGB2HSV(&palette, &palette_hsv);
}

void PaletteManager::NextWalkStyle() {
  if(walkStyle == WS_BLEND_SLOW || walkStyle == WS_BLEND_FAST) {
    // Undo the partial transition into the next palette
    curPalette = curPalette==0 ? NUM_PALETTES-1 : curPalette-1;
  }
  walkStyle = (walkStyle+1) % NUM_WALK_STYLES;

  StartPaletteTransition(); // Transition back to unadulturated palette
}

void PaletteManager::NextPalette() {
  curPalette = (curPalette+1) % NUM_PALETTES;
  StartPaletteTransition();
}

void PaletteManager::StartPaletteTransition() {
  targetPalette = GetPalette(curPalette);
  CHSVPalette16 temp;
  PaletteRGB2HSV(&targetPalette, &temp);
  targetPalette = temp;
  switchingPalette = true;
  updateInterval = INTERVAL_PALETTE_SWITCHING;
}

void PaletteManager::Update() {
  if(switchingPalette) {
    nblendPaletteTowardPalette(palette, targetPalette);
    if(palette == targetPalette) {
      switchingPalette = false;
      PaletteRGB2HSV(&palette, &palette_hsv);
      SetUpdateInterval();
    }
  }
  else {
    // Do walking logic if not switching palettes
    switch(walkStyle) {
      case WS_SWITCH_SLOW:
      case WS_SWITCH_FAST:
        NextPalette();
        break;
      
      case WS_BLEND_SLOW:
      case WS_BLEND_FAST:
        NextPalette();
        SetUpdateInterval();
        break;
        
      case WS_RAND_SLOW:
      case WS_RAND_FAST:
        if(random8(2) == 0)
          for(CHSV & pixel : palette_hsv.entries) { pixel.hue++;}
        else
          for(CHSV & pixel : palette_hsv.entries) { pixel.hue--;}
        break;
        
      case WS_CYCLE_SLOW:
      case WS_CYCLE_MED:
      case WS_CYCLE_FAST:
        for(CHSV & pixel : palette_hsv.entries) { pixel.hue++;}
        break;
    }
  
    palette = palette_hsv;
  }
}

void PaletteManager::SetUpdateInterval() {
  switch(walkStyle) {
    case WS_SWITCH_SLOW:
      updateInterval = 10000;
      break;
    case WS_SWITCH_FAST:
      updateInterval = 4000;
      break;
    case WS_BLEND_SLOW:
      updateInterval = 50;
      break;
    case WS_RAND_SLOW:
      updateInterval = 13;
      break;
    case WS_CYCLE_SLOW:
      updateInterval = 50;
      break;
    case WS_CYCLE_MED:
      updateInterval = 20;
      break;
    case WS_BLEND_FAST:
      updateInterval = 15;
      break;
    case WS_CYCLE_FAST:
      updateInterval = 8;
      break;
    case WS_RAND_FAST:
      updateInterval = 5;
      break;
  }
}

CHSV PaletteManager::GetRandomColor(uint8_t brightness) {
  uint8_t index = random8(4);
  CHSV randColor = ColorFromPalette(palette_hsv, 128 + index * 64);
  if(brightness) randColor.value = brightness;
  return randColor;
}

CRGBPalette16 PaletteManager::GetPalette(uint8_t paletteName) {
  switch(paletteName) {
    case PN_OCEAN:
      return (CRGBPalette16)OceanColors_p;
    case PN_LAVA:
      return (CRGBPalette16)LavaColors_p;
    case PN_FOREST:
      return (CRGBPalette16)ForestColors_p;
    case PN_CLOUD:
      return (CRGBPalette16)CloudColors_p;
    case PN_PARTY:
      return (CRGBPalette16)PartyColors_p;
    case PN_RAINBOW:
      return (CRGBPalette16)RainbowStripesColors_p;
    case PN_HEAT:
      return (CRGBPalette16)HeatColors_p;
    case PN_PASTEL:
      return (CRGBPalette16)PastelColors_p;
    case PN_LOVE:
      return (CRGBPalette16)LoveColors_p;
    case PN_STROBE:
      return (CRGBPalette16)StrobeColors_p;
  }
}

void PaletteManager::PaletteRGB2HSV(CRGBPalette16* rgbPalette, CHSVPalette16* hsvPalette) {
  for(uint8_t i = 0; i < 16; i++)
    hsvPalette->entries[i] = rgb2hsv_approximate(rgbPalette->entries[i]);
}

