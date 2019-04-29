#ifndef PALETTE_MANAGER_H
#define PALETTE_MANAGER_H

#include "PaletteManager.cfg"

class PaletteManager {
  public:
	/// Fields
	CHSV palette[PALETTE_SIZE];
	
	/// Object management
    PaletteManager(); 
    void Init(uint32_t* _curTime, uint32_t intialWalkLength, uint32_t intialPauseLength, uint8_t initialPalette);
    void SkipTime(uint32_t amount);
	
    /// Accessors
	uint8_t GetTarget();
    void SetTarget(uint8_t newTarget);
    uint32_t GetWalkLength();
    void SetWalkLength(uint32_t newWalkLength);
    uint32_t GetPauseLength();
    void SetPauseLength(uint32_t newPauseLength);

	/// Logic
    void Update();
    void NextPalette();


  private:
	uint32_t* curTime;
    uint8_t target;
    uint32_t walkLength;
    uint32_t pauseLength;
    CHSV oldPalette[PALETTE_SIZE];
    uint32_t lastSwitchTime;
};
#endif

