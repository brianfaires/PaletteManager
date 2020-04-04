#pragma once
#include "PaletteManagerConfig.h"

class PaletteManager {
  public:
	/// Fields
	CHSV palette[PALETTE_SIZE];
	
	/// Object management
  PaletteManager(); 
  void Init(uint32_t* _curTime, uint32_t intialWalkLength, uint32_t intialPauseLength, PaletteIndex initialPalette);
  void SkipTime(uint32_t amount);
	
    /// Accessors
	PaletteIndex getTarget();
  void setTarget(PaletteIndex newTarget);

  uint32_t getWalkLength();
  void setWalkLength(uint32_t newWalkLength);
  
  uint32_t getPauseLength();
  void setPauseLength(uint32_t newPauseLength);

	/// Logic
  void Update();
  void NextPalette();


  private:
	  uint32_t* curTime;
    PaletteIndex target;
    uint32_t walkLength;
    uint32_t pauseLength;
    
    CHSV oldPalette[PALETTE_SIZE];
    uint32_t lastSwitchTime;
};
