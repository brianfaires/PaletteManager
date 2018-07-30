#ifndef PALETTE_MANAGER_H
#define PALETTE_MANAGER_H

#include "PaletteManager.cfg"

class PaletteManager {
  public:
    CHSV palette[PALETTE_SIZE];

    PaletteManager(); 
    void Init(uint32_t intialWalkLength, uint32_t intialPauseLength, uint32_t curTime);
    void Update(uint32_t curTime);
    void NextPalette(uint32_t curTime);
    void SkipTime(uint32_t amount);
    
    uint8_t GetTarget();
    void SetTarget(uint8_t newTarget, uint32_t curTime);
    
    uint32_t GetWalkLength();
    void SetWalkLength(uint32_t newWalkLength, uint32_t curTime);
    
    uint32_t GetPauseLength();
    void SetPauseLength(uint32_t newPauseLength, uint32_t curTime);

  private:
    uint8_t target;
    uint32_t walkLength;
    uint32_t pauseLength;
    CHSV oldPalette[PALETTE_SIZE];
    CHSV targetPalette[PALETTE_SIZE];
    uint32_t lastSwitchTime;
};
#endif

