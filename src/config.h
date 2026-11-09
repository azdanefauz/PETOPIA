#ifndef CONFIG_H
#define CONFIG_H

#include "raylib.h"

/**
 * ============================================================================
 * PETOPIA - GAME & ASSET CONFIGURATION
 * ============================================================================
 * [EDITABLE]: This file contains all file paths, layout constants, and 
 * animation settings. Edit any value here to customize the game.
 * ============================================================================
 */

// ----------------------------------------------------------------------------
// [EDITABLE]: Window & Display Settings
// ----------------------------------------------------------------------------
#define SCREEN_WIDTH            800
#define SCREEN_HEIGHT           600
#define TARGET_FPS              60

// ----------------------------------------------------------------------------
// [EDITABLE]: Font Configuration
// Drop your custom TTF or OTF font here to replace the default Raylib font!
// ----------------------------------------------------------------------------
#define PATH_FONT               "assets/fonts/game_font.ttf"
#define FONT_SIZE_DEFAULT       20

// ----------------------------------------------------------------------------
// [EDITABLE]: Asset Paths - Organized by Category
// ----------------------------------------------------------------------------

// Logo
#define PATH_LOGO               "assets/images/logo/logo.png"

// Pet Sprites & Avatars (Static PNG or Spritesheet)
#define PATH_PET_CAT            "assets/images/pets/cat.png"
#define PATH_PET_DOG            "assets/images/pets/dog.png"
#define PATH_PET_RABBIT         "assets/images/pets/rabbit.png"
#define PATH_PET_NANDA          "assets/images/pets/nanda.png"
#define PATH_PET_ILLUST         "assets/images/pets/pet_illustration.png"

// Room Backgrounds
#define PATH_BG_LIVINGROOM      "assets/images/backgrounds/livingroom.png"
#define PATH_BG_KITCHEN         "assets/images/backgrounds/kitchen.png"
#define PATH_BG_BACKYARD        "assets/images/backgrounds/backyard.png"
#define PATH_BG_BEDROOM         "assets/images/backgrounds/bedroom.png"

// ----------------------------------------------------------------------------
// [EDITABLE]: Animation Settings for Future Spritesheets
// When your animator delivers spritesheets (horizontal strips of frames):
// Change PET_SPRITE_FRAMES to your frame count (e.g., 4 or 6 frames)
// ----------------------------------------------------------------------------
#define PET_SPRITE_FRAMES       1       // [EDITABLE]: 1 = static image, >1 = animated spritesheet
#define PET_ANIM_FPS            6       // [EDITABLE]: Animation speed (frames per second)

// ----------------------------------------------------------------------------
// [EDITABLE]: Gameplay & Stat Decay Settings
// ----------------------------------------------------------------------------
#define STAT_DECAY_INTERVAL     4.0     // [EDITABLE]: Seconds between each stat decay tick
#define MAX_NAME_LENGTH         12      // [EDITABLE]: Maximum characters in pet name

#endif
