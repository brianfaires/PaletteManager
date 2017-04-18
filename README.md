# PaletteManager
A wrapper for FastLED's Palettes, adjustable walk rates/various walk styles, and my collection of premade palettes to share across my LED projects.

The goal of this project is twofold:
  1. To have one central location to form premade color schemes, that will then be used across several projects that I would one day like to sync up.
  2. To seperate the color scheme entirely from the rest of the logic. All that is needed to manage color schemes are occasional calls indicating that a given internal mode should be incremented (current palette, walk style, etc).

The interface has been kept intentionally limited, as all devices using this library will have limited user IO.
