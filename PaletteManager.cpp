#include "PaletteManager.h"

PaletteManager::PaletteManager() { }

uint8_t PaletteManager::GetTarget() { return target; }
void PaletteManager::SetTarget(uint8_t newTarget, uint32_t curTime) {
  target = newTarget % NUM_PALETTES;
  memcpy(oldPalette, palette, sizeof(CHSV)*PALETTE_SIZE);
  memcpy(targetPalette, allPalettes[target], sizeof(CHSV)*PALETTE_SIZE);
  lastSwitchTime = curTime;
}

uint32_t PaletteManager::GetWalkLength() { return walkLength; }
void PaletteManager::SetWalkLength(uint32_t newWalkLength, uint32_t curTime) {
  walkLength = newWalkLength;
  memcpy(oldPalette, palette, sizeof(CHSV)*PALETTE_SIZE);
  lastSwitchTime = curTime;
}

uint32_t PaletteManager::GetPauseLength() { return pauseLength; }
void PaletteManager::SetPauseLength(uint32_t newPauseLength, uint32_t curTime) {
  memcpy(oldPalette, palette, sizeof(CHSV)*PALETTE_SIZE);
  
  if(curTime - lastSwitchTime <= pauseLength) {
    // Haven't started blending yet
    if(curTime - lastSwitchTime <= newPauseLength) {
      // Just update the timer, neither one has tripped yet
    }
    else {
      // Start blending immediately
      lastSwitchTime - curTime - newPauseLength;
    }
  }
  else {
    // Middle of blending, offset by difference to keep blend the same
    lastSwitchTime -= (newPauseLength - pauseLength);
  }

  pauseLength = newPauseLength;
}

void PaletteManager::Init(uint32_t initialWalkLength, uint32_t intialPauseLength, uint32_t curTime) {
  walkLength = initialWalkLength;
  pauseLength = intialPauseLength;
  lastSwitchTime = curTime;
  memcpy(palette, allPalettes[0], sizeof(CHSV)*PALETTE_SIZE);
  memcpy(oldPalette, allPalettes[0], sizeof(CHSV)*PALETTE_SIZE);
  target = 1 % NUM_PALETTES;
  memcpy(targetPalette, allPalettes[target], sizeof(CHSV)*PALETTE_SIZE);
}

void PaletteManager::SkipTime(uint32_t amount) {
  lastSwitchTime += amount;
}

void PaletteManager::Update(uint32_t curTime) {
  if(curTime - lastSwitchTime >= pauseLength) {
    uint32_t transitionTime = curTime - lastSwitchTime - pauseLength;
    if(transitionTime < walkLength) {
      uint8_t blendAmount = 255 * transitionTime / walkLength;//debug: changed from 256, why?
      for(uint8_t i = 0; i < PALETTE_SIZE; i++) {
        palette[i] = blend(oldPalette[i], targetPalette[i], blendAmount, /*gradientProtection ? blendDirections[i] :*/ SHORTEST_HUES);
      }
    }
    else {
      // Blending just finished
      memcpy(palette, targetPalette, sizeof(CHSV)*PALETTE_SIZE);
      target = (target + 1) % NUM_PALETTES;
      memcpy(targetPalette, allPalettes[target], sizeof(CHSV)*PALETTE_SIZE);
      memcpy(oldPalette, palette, sizeof(CHSV)*PALETTE_SIZE);
      lastSwitchTime = curTime;
    }
  }
  // TODO: Alternatively, directly access the pointer and get rid of target
  //targetPalette += sizeof(CHSV);
  //if(targetPalette == allPalettes[sizeof(CHSV)*NUM_PALETTES]) { targetPalette -= sizeof(CHSV)*NUM_PALETTES; }

  #ifdef USE_TEST_PALETTE
    memcpy(palette, test_palette, sizeof(CHSV)*PALETTE_SIZE);
    memcpy(oldPalette, test_palette, sizeof(CHSV)*PALETTE_SIZE);
    memcpy(targetPalette, test_palette, sizeof(CHSV)*PALETTE_SIZE);
  #endif
}

void PaletteManager::NextPalette(uint32_t curTime) {
  memcpy(oldPalette, targetPalette, sizeof(CHSV)*PALETTE_SIZE);
  memcpy(palette, targetPalette, sizeof(CHSV)*PALETTE_SIZE);
  target = (target + 1) % NUM_PALETTES;
  memcpy(targetPalette, allPalettes[target], sizeof(CHSV)*PALETTE_SIZE);
  lastSwitchTime = curTime;
}

