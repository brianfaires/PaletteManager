#include "PaletteManager.h"

/// Object management
PaletteManager::PaletteManager() { }
void PaletteManager::Init(uint32_t* _curTime, uint32_t initialWalkLength, uint32_t intialPauseLength, PaletteIndex initialPalette) {
  curTime = _curTime;
  walkLength = initialWalkLength;
  pauseLength = intialPauseLength;
  target = initialPalette;
  NextPalette(); // Loads initialPalette, sets next target, and resets timer
}
void PaletteManager::SkipTime(uint32_t amount) {
  lastSwitchTime += amount;
}

/// Accessors
PaletteIndex PaletteManager::getTarget() { return target; }
void PaletteManager::setTarget(PaletteIndex newTarget) {
  target = static_cast<PaletteIndex>(static_cast<uint8_t>(newTarget) % NUM_PALETTES);
  
  if(*curTime - lastSwitchTime > pauseLength) {
    // Already blending. Reset target and start blending from current palette.
    memcpy(oldPalette, palette, sizeof(CHSV)*PALETTE_SIZE);
    lastSwitchTime = *curTime - pauseLength;
  }
}

uint32_t PaletteManager::getWalkLength() { return walkLength; }
void PaletteManager::setWalkLength(uint32_t newWalkLength) {
  if(*curTime - lastSwitchTime > pauseLength) {
    // Already blending. Reset target and restart blending. (old method)
    //memcpy(oldPalette, palette, sizeof(CHSV)*PALETTE_SIZE);
    //lastSwitchTime = *curTime - pauseLength;
    
    // Already blending.  Determine what % has been done and create same percentage in the new timer
    uint32_t blendTime = *curTime - lastSwitchTime - pauseLength;
    uint64_t percBlend = uint64_t(blendTime) * 0x10000 / uint64_t(walkLength);
    uint64_t newBlendTime = uint64_t(percBlend) * uint64_t(newWalkLength) / 0x10000;
    lastSwitchTime = *curTime - pauseLength - newBlendTime;
  }

  walkLength = newWalkLength;
}

uint32_t PaletteManager::getPauseLength() { return pauseLength; }
void PaletteManager::setPauseLength(uint32_t newPauseLength) {
  //memcpy(oldPalette, palette, sizeof(CHSV)*PALETTE_SIZE); // Todo:This seems unnecessary, right? Even interferes with the else statement
  
  if(*curTime - lastSwitchTime <= pauseLength) {
    // Haven't started blending yet
    if(*curTime - lastSwitchTime > newPauseLength) {
      // Start blending immediately; but start at 0
      lastSwitchTime = *curTime - newPauseLength;
    }
  }
  else {
    // Middle of blending, offset by difference to keep blend the same
    lastSwitchTime -= (newPauseLength - pauseLength);
  }

  pauseLength = newPauseLength;
}

/// Logic
void PaletteManager::Update() {
  CHSV* targetPalette = allPalettes[static_cast<uint8_t>(target)];

  if(*curTime - lastSwitchTime >= pauseLength) {
    // Currently transitioning
    uint32_t transitionTime = *curTime - lastSwitchTime - pauseLength;
    if(transitionTime < walkLength) {
      fract8 blendAmount = 0x100 * transitionTime / walkLength;
      for(uint8_t i = 0; i < PALETTE_SIZE; i++) {
        palette[i] = blend(oldPalette[i], targetPalette[i], blendAmount, SHORTEST_HUES);
      }
    }
    else {
      // Blending just finished
      NextPalette();
    }
  }
}

void PaletteManager::NextPalette() {
  memcpy(oldPalette, allPalettes[(uint8_t)target], sizeof(CHSV)*PALETTE_SIZE);
  memcpy(palette, allPalettes[(uint8_t)target], sizeof(CHSV)*PALETTE_SIZE);
  target = static_cast<PaletteIndex>((static_cast<uint8_t>(target) + 1) % NUM_PALETTES);
  lastSwitchTime = *curTime;
}

