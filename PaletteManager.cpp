#include "PaletteManager.h"

/// Object management
PaletteManager::PaletteManager() { }
void PaletteManager::Init(uint32_t* _curTime, uint32_t initialWalkLength, uint32_t intialPauseLength, uint8_t initialPalette) {
  curTime = _curTime;
  walkLength = initialWalkLength;
  pauseLength = intialPauseLength;
  lastSwitchTime = *curTime;
  memcpy(palette, allPalettes[0], sizeof(CHSV)*PALETTE_SIZE);
  memcpy(oldPalette, allPalettes[0], sizeof(CHSV)*PALETTE_SIZE);
  target = initialPalette % NUM_PALETTES;
}
void PaletteManager::SkipTime(uint32_t amount) {
  lastSwitchTime += amount;
}

/// Accessors
uint8_t PaletteManager::GetTarget() { return target; }
void PaletteManager::SetTarget(uint8_t newTarget) {
  target = newTarget % NUM_PALETTES;
  
  if(*curTime - lastSwitchTime > pauseLength) {
    // Already blending. Reset target and restart blending.
	memcpy(oldPalette, palette, sizeof(CHSV)*PALETTE_SIZE);
	lastSwitchTime = *curTime - pauseLength;
  }
}

uint32_t PaletteManager::GetWalkLength() { return walkLength; }
void PaletteManager::SetWalkLength(uint32_t newWalkLength) {
  walkLength = newWalkLength;
  
  if(*curTime - lastSwitchTime > pauseLength) {
	// Already blending. Reset target and restart blending. // todo: calc the % of blend completed and adjust lastSwitchTime to match it
	memcpy(oldPalette, palette, sizeof(CHSV)*PALETTE_SIZE);
	lastSwitchTime = *curTime - pauseLength;
  }
}

uint32_t PaletteManager::GetPauseLength() { return pauseLength; }
void PaletteManager::SetPauseLength(uint32_t newPauseLength) {
  memcpy(oldPalette, palette, sizeof(CHSV)*PALETTE_SIZE);
  
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
  if(*curTime - lastSwitchTime >= pauseLength) {
	uint32_t transitionTime = *curTime - lastSwitchTime - pauseLength;
	if(transitionTime < walkLength) {
	  uint8_t blendAmount = 255 * transitionTime / walkLength;
	  for(uint8_t i = 0; i < PALETTE_SIZE; i++) {
		palette[i] = blend(oldPalette[i], allPalettes[target][i], blendAmount, /*gradientProtection ? blendDirections[i] :*/ SHORTEST_HUES);
	  }
	}
	else {
	  // Blending just finished
	  memcpy(palette, allPalettes[target], sizeof(CHSV)*PALETTE_SIZE);
	  target = (target + 1) % NUM_PALETTES;
	  memcpy(oldPalette, palette, sizeof(CHSV)*PALETTE_SIZE);
	  lastSwitchTime = *curTime;
	}
  }
  // todo: Alternatively, directly access the pointer and get rid of target
  //targetPalette += sizeof(CHSV)*PALETTE_SIZE;
  //if(targetPalette == allPalettes[sizeof(CHSV)*NUM_PALETTES]) { targetPalette -= sizeof(CHSV)*NUM_PALETTES; }
}

void PaletteManager::NextPalette() {
  memcpy(oldPalette, allPalettes[target], sizeof(CHSV)*PALETTE_SIZE);
  memcpy(palette, allPalettes[target], sizeof(CHSV)*PALETTE_SIZE);
  target = (target + 1) % NUM_PALETTES;
  lastSwitchTime = *curTime;
}

