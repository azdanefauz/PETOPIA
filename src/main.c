/**
 * ============================================================================
 * PETOPIA - Virtual Pet Game (Enhanced & Organized Edition)
 * Original project by: zakyqwert
 * ============================================================================
 * 
 * MODULAR ARCHITECTURE:
 *   - config.h   : Asset file paths, fonts, animation settings & gameplay constants
 *   - strings.h  : All editable game text, titles, subtitles & messages
 *   - pet.h      : Pet struct and types
 *   - assets/    : Subdirectories for fonts, logo, pets, backgrounds, and ui
 * ============================================================================
 */

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "raylib.h"
#include "pet.h"
#include "config.h"   // [EDITABLE]: Paths, fonts, animation & constants
#include "strings.h"  // [EDITABLE]: All user-facing text

// ----------------------------------------------------------------------------
// GAME SCREENS ENUMERATION
// ----------------------------------------------------------------------------
typedef enum {
    SCREEN_WELCOME,    // Welcome / Main menu
    SCREEN_SELECT,     // Choose pet character
    SCREEN_NAME,       // Input pet name
    SCREEN_MAIN_ROOM,  // Living room (central room)
    SCREEN_KITCHEN,    // Kitchen (feed)
    SCREEN_BEDROOM,    // Bedroom (rest)
    SCREEN_BACKYARD    // Backyard (play)
} Screen;

// ----------------------------------------------------------------------------
// ASSET MANAGEMENT STRUCT
// Holds loaded textures and custom font.
// ----------------------------------------------------------------------------
typedef struct {
    Font font;
    bool hasCustomFont;
    Texture2D logo;
    Texture2D petAvatars[4];
    Texture2D petIllustration;
    Texture2D bgLivingRoom;
    Texture2D bgKitchen;
    Texture2D bgBackyard;
    Texture2D bgBedroom;
} GameAssets;

// ----------------------------------------------------------------------------
// HELPER FUNCTIONS
// ----------------------------------------------------------------------------

static int ClampStat(int value, int minVal, int maxVal) {
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

// Check primary path or fallback path
static Texture2D LoadTextureWithFallback(const char *primaryPath, const char *fallbackPath) {
    if (FileExists(primaryPath)) return LoadTexture(primaryPath);
    if (fallbackPath != NULL && FileExists(fallbackPath)) return LoadTexture(fallbackPath);
    return (Texture2D){ 0 };
}

// Interactive button component
static bool ButtonEx(Rectangle r, const char *text, Color baseColor, Color hoverColor, int fontSize) {
    bool hover = CheckCollisionPointRec(GetMousePosition(), r);
    DrawRectangleRec(r, hover ? hoverColor : baseColor);
    DrawRectangleLinesEx(r, 2, DARKGRAY);
    int w = MeasureText(text, fontSize);
    DrawText(text, (int)(r.x + (r.width - w) / 2), (int)(r.y + (r.height - fontSize) / 2), fontSize, BLACK);
    return hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

static bool Button(Rectangle r, const char *text) {
    return ButtonEx(r, text, LIGHTGRAY, SKYBLUE, 20);
}

static const char *PetLabel(PetType t) {
    switch (t) {
        case PET_CAT:    return TXT_PET_CAT;
        case PET_DOG:    return TXT_PET_DOG;
        case PET_RABBIT: return TXT_PET_RABBIT;
        case PET_NANDA:  return TXT_PET_NANDA;
        default:         return "";
    }
}

static Color PetColor(PetType t) {
    switch (t) {
        case PET_CAT:    return ORANGE;
        case PET_DOG:    return BROWN;
        case PET_RABBIT: return PINK;
        case PET_NANDA:  return BLUE;
        default:         return GRAY;
    }
}

static int PetIndex(PetType t) {
    switch (t) {
        case PET_CAT:    return 0;
        case PET_DOG:    return 1;
        case PET_RABBIT: return 2;
        case PET_NANDA:  return 3;
        default:         return -1;
    }
}

/**
 * Draws either a loaded texture or a clean border container.
 * Absolutely NO text is drawn inside to avoid overlaps.
 */
static void DrawAssetOrBox(Texture2D tex, Rectangle r, Color placeholderTint) {
    if (tex.id > 0) {
        Rectangle src = { 0.0f, 0.0f, (float)tex.width, (float)tex.height };
        DrawTexturePro(tex, src, r, (Vector2){ 0, 0 }, 0.0f, WHITE);
        DrawRectangleLinesEx(r, 2, DARKGRAY);
    } else {
        DrawRectangleRec(r, (Color){ placeholderTint.r, placeholderTint.g, placeholderTint.b, 45 });
        DrawRectangleLinesEx(r, 2, placeholderTint);
        DrawLine((int)r.x, (int)r.y, (int)r.x + 12, (int)r.y + 12, placeholderTint);
        DrawLine((int)(r.x + r.width), (int)r.y, (int)(r.x + r.width - 12), (int)r.y + 12, placeholderTint);
    }
}

/**
 * ANIMATION-READY PET SPRITE RENDERER
 * ---------------------------------------------------------------------------
 * Supports:
 *   1. Custom horizontal spritesheet animation (sliced via PET_SPRITE_FRAMES)
 *   2. Static single-image sprite (when PET_SPRITE_FRAMES == 1)
 *   3. Clean procedural fallback face (zero overlapping text)
 */
static void DrawPetAvatarAnimated(Rectangle r, PetType t, Texture2D customTex, int animFrame) {
    if (customTex.id > 0) {
        int totalFrames = (PET_SPRITE_FRAMES > 0) ? PET_SPRITE_FRAMES : 1;
        float frameWidth = (float)customTex.width / (float)totalFrames;
        int currentFrame = animFrame % totalFrames;

        // Slice horizontal frame from spritesheet
        Rectangle src = { currentFrame * frameWidth, 0.0f, frameWidth, (float)customTex.height };
        DrawTexturePro(customTex, src, r, (Vector2){ 0, 0 }, 0.0f, WHITE);
        DrawRectangleLinesEx(r, 2, DARKBLUE);
    } else {
        Color col = PetColor(t);
        DrawRectangleRec(r, col);
        DrawRectangleLinesEx(r, 2, DARKBLUE);

        int cx = (int)(r.x + r.width / 2);
        int cy = (int)(r.y + r.height / 2);
        
        // Clean procedural face (no text clutter)
        DrawCircle(cx - 18, cy - 12, 5, WHITE);
        DrawCircle(cx - 18, cy - 12, 2, BLACK);
        DrawCircle(cx + 18, cy - 12, 5, WHITE);
        DrawCircle(cx + 18, cy - 12, 2, BLACK);
        DrawCircle(cx, cy + 4, 3, MAROON);
        DrawLine(cx - 6, cy + 12, cx + 6, cy + 12, BLACK);
    }
}

/**
 * Draws stat bar with numeric ratio indicator.
 */
static void DrawStatBar(int x, int y, int width, int height, const char *label, int value, Color barColor) {
    DrawText(label, x, y, 16, BLACK);
    Rectangle bg = { (float)x + 115, (float)y, (float)width, (float)height };
    DrawRectangleRec(bg, LIGHTGRAY);
    DrawRectangleLinesEx(bg, 1, DARKGRAY);

    float fillWidth = (float)width * ((float)value / 100.0f);
    Rectangle fill = { (float)x + 115, (float)y, fillWidth, (float)height };
    DrawRectangleRec(fill, barColor);

    DrawText(TextFormat("%d/100", value), x + 125 + width, y, 14, DARKGRAY);
}

// ----------------------------------------------------------------------------
// MAIN PROGRAM
// ----------------------------------------------------------------------------
int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TXT_WINDOW_TITLE);
    SetTargetFPS(TARGET_FPS);

    // Load assets from organized folders (with backward-compatible fallbacks)
    GameAssets assets = { 0 };

    if (FileExists(PATH_FONT)) {
        assets.font = LoadFont(PATH_FONT);
        assets.hasCustomFont = true;
    }

    assets.logo            = LoadTextureWithFallback(PATH_LOGO, FileExists("petopia.png") ? "petopia.png" : "assets/logo.png");
    assets.petAvatars[0]   = LoadTextureWithFallback(PATH_PET_CAT, FileExists("cat selection.png") ? "cat selection.png" : "assets/cat.png");
    assets.petAvatars[1]   = LoadTextureWithFallback(PATH_PET_DOG, FileExists("dog selection.png") ? "dog selection.png" : "assets/dog.png");
    assets.petAvatars[2]   = LoadTextureWithFallback(PATH_PET_RABBIT, FileExists("rabbit selection.png") ? "rabbit selection.png" : "assets/rabbit.png");
    assets.petAvatars[3]   = LoadTextureWithFallback(PATH_PET_NANDA, "assets/nanda.png");
    assets.petIllustration = LoadTextureWithFallback(PATH_PET_ILLUST, "assets/pet_illustration.png");
    assets.bgLivingRoom    = LoadTextureWithFallback(PATH_BG_LIVINGROOM, "assets/bg_livingroom.png");
    assets.bgKitchen       = LoadTextureWithFallback(PATH_BG_KITCHEN, "assets/bg_kitchen.png");
    assets.bgBackyard      = LoadTextureWithFallback(PATH_BG_BACKYARD, "assets/bg_backyard.png");
    assets.bgBedroom       = LoadTextureWithFallback(PATH_BG_BEDROOM, "assets/bg_bedroom.png");

    Screen currentScreen = SCREEN_WELCOME;
    Pet pet = { PET_NONE, "", 80, 80, 80, 80, 90 };

    double lastDecayTime = GetTime();
    char messageLog[64] = TXT_MSG_WELCOME;
    double messageTime = GetTime();

    // Animation frame tracking
    int animFrame = 0;
    double lastAnimTime = GetTime();

    // ------------------------------------------------------------------------
    // MAIN LOOP
    // ------------------------------------------------------------------------
    while (!WindowShouldClose()) {
        double now = GetTime();

        // Update animation frames based on configured FPS
        if ((now - lastAnimTime) >= (1.0 / (double)PET_ANIM_FPS)) {
            lastAnimTime = now;
            animFrame++;
        }

        // Real-time Stat Decay
        if (currentScreen >= SCREEN_MAIN_ROOM && (now - lastDecayTime) >= STAT_DECAY_INTERVAL) {
            lastDecayTime = now;
            pet.hunger = ClampStat(pet.hunger - 1, 0, 100);
            pet.cleanliness = ClampStat(pet.cleanliness - 1, 0, 100);

            if (currentScreen != SCREEN_BEDROOM) {
                pet.energy = ClampStat(pet.energy - 1, 0, 100);
            }

            if (pet.hunger < 25 || pet.cleanliness < 25) {
                pet.happiness = ClampStat(pet.happiness - 2, 0, 100);
                pet.health = ClampStat(pet.health - 1, 0, 100);
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // ====================================================================
        // SCREEN 1: WELCOME SCREEN (No text overlap)
        // ====================================================================
        if (currentScreen == SCREEN_WELCOME) {
            ClearBackground((Color){ 245, 248, 255, 255 });

            int titleW = MeasureText(TXT_WELCOME_TITLE, 56);
            DrawText(TXT_WELCOME_TITLE, (SCREEN_WIDTH - titleW) / 2, 50, 56, DARKBLUE);

            int subW = MeasureText(TXT_WELCOME_SUBTITLE, 22);
            DrawText(TXT_WELCOME_SUBTITLE, (SCREEN_WIDTH - subW) / 2, 120, 22, DARKGRAY);

            Rectangle logoBox = { 250, 175, 300, 180 };
            DrawAssetOrBox(assets.logo, logoBox, DARKBLUE);

            if (Button((Rectangle){ 325, 395, 150, 50 }, TXT_BTN_START)) {
                currentScreen = SCREEN_SELECT;
            }
        }

        // ====================================================================
        // SCREEN 2: PET SELECTION SCREEN
        // ====================================================================
        else if (currentScreen == SCREEN_SELECT) {
            ClearBackground((Color){ 245, 245, 250, 255 });

            int selTitleW = MeasureText(TXT_SELECT_TITLE, 30);
            DrawText(TXT_SELECT_TITLE, (SCREEN_WIDTH - selTitleW) / 2, 35, 30, DARKBLUE);

            int selSubW = MeasureText(TXT_SELECT_SUBTITLE, 16);
            DrawText(TXT_SELECT_SUBTITLE, (SCREEN_WIDTH - selSubW) / 2, 75, 16, DARKGRAY);

            PetType types[4] = { PET_CAT, PET_DOG, PET_RABBIT, PET_NANDA };

            for (int i = 0; i < 4; i++) {
                Rectangle card = { 50.0f + i * 180, 120, 160, 330 };
                
                if (pet.type == types[i]) {
                    DrawRectangleRec(card, (Color){ 220, 235, 255, 255 });
                    DrawRectangleLinesEx(card, 3, DARKBLUE);
                } else {
                    DrawRectangleRec(card, WHITE);
                    DrawRectangleLinesEx(card, 1, LIGHTGRAY);
                }

                Rectangle avatarRec = { card.x + 20, card.y + 15, 120, 120 };
                int pIdx = PetIndex(types[i]);
                Texture2D avTex = (pIdx >= 0) ? assets.petAvatars[pIdx] : (Texture2D){ 0 };
                DrawPetAvatarAnimated(avatarRec, types[i], avTex, animFrame);

                const char *pName = PetLabel(types[i]);
                int nw = MeasureText(pName, 20);
                DrawText(pName, (int)(card.x + (card.width - nw) / 2), (int)card.y + 155, 20, BLACK);

                const char *btnLabel = (pet.type == types[i]) ? TXT_BTN_SELECTED : TXT_BTN_SELECT;
                Color btnBg = (pet.type == types[i]) ? SKYBLUE : LIGHTGRAY;
                if (ButtonEx((Rectangle){ card.x + 25, card.y + 200, 110, 42 }, btnLabel, btnBg, SKYBLUE, 16)) {
                    pet.type = types[i];
                }
            }

            if (pet.type != PET_NONE) {
                if (ButtonEx((Rectangle){ 325, 485, 150, 48 }, TXT_BTN_CONTINUE, GREEN, LIME, 20)) {
                    currentScreen = SCREEN_NAME;
                }
            }
        }

        // ====================================================================
        // SCREEN 3: NAME YOUR PET SCREEN
        // ====================================================================
        else if (currentScreen == SCREEN_NAME) {
            ClearBackground((Color){ 245, 250, 245, 255 });

            int nameTitleW = MeasureText(TXT_NAME_TITLE, 30);
            DrawText(TXT_NAME_TITLE, (SCREEN_WIDTH - nameTitleW) / 2, 35, 30, DARKBLUE);

            Rectangle illustRec = { 280, 85, 240, 180 };
            int pIdx = PetIndex(pet.type);
            Texture2D pTex = (pIdx >= 0) ? assets.petAvatars[pIdx] : (Texture2D){ 0 };
            if (assets.petIllustration.id > 0) pTex = assets.petIllustration;

            DrawAssetOrBox(pTex, illustRec, PetColor(pet.type));
            if (pTex.id == 0) {
                DrawPetAvatarAnimated((Rectangle){ 330, 115, 140, 120 }, pet.type, (Texture2D){ 0 }, animFrame);
            }

            int key = GetCharPressed();
            while (key > 0) {
                int len = (int)strlen(pet.name);
                if (key >= 32 && key <= 125 && len < MAX_NAME_LENGTH) {
                    pet.name[len] = (char)key;
                    pet.name[len + 1] = '\0';
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE)) {
                int len = (int)strlen(pet.name);
                if (len > 0) pet.name[len - 1] = '\0';
            }

            Rectangle inputField = { 250, 295, 300, 50 };
            DrawRectangleRec(inputField, WHITE);
            DrawRectangleLinesEx(inputField, 2, DARKBLUE);
            DrawText(pet.name, 265, 307, 24, BLACK);
            if (strlen(pet.name) == 0) {
                DrawText(TXT_NAME_PLACEHOLDER, 265, 310, 18, LIGHTGRAY);
            }

            if (strlen(pet.name) > 0) {
                if (ButtonEx((Rectangle){ 325, 375, 150, 50 }, TXT_BTN_ENTER_HOUSE, GREEN, LIME, 18)) {
                    currentScreen = SCREEN_MAIN_ROOM;
                }
            }
        }

        // ====================================================================
        // SCREEN 4: LIVING ROOM (Central Hub)
        // ====================================================================
        else if (currentScreen == SCREEN_MAIN_ROOM) {
            Rectangle bgRec = { 20, 20, 760, 350 };
            DrawAssetOrBox(assets.bgLivingRoom, bgRec, DARKGRAY);
            if (assets.bgLivingRoom.id == 0) {
                DrawRectangleRec((Rectangle){ 20, 250, 760, 120 }, (Color){ 220, 200, 170, 160 });
            }

            int pIdx = PetIndex(pet.type);
            Texture2D avTex = (pIdx >= 0) ? assets.petAvatars[pIdx] : (Texture2D){ 0 };
            DrawPetAvatarAnimated((Rectangle){ 330, 160, 140, 140 }, pet.type, avTex, animFrame);
            
            int nameW = MeasureText(pet.name, 24);
            DrawText(pet.name, (SCREEN_WIDTH - nameW) / 2, 120, 24, DARKBLUE);

            DrawText(TextFormat("%s si %s", pet.name, PetLabel(pet.type)), 40, 35, 20, DARKBLUE);

            int statX = 40;
            int statY = 390;
            DrawStatBar(statX, statY, 140, 15, TXT_STAT_HUNGER, pet.hunger, ORANGE);
            DrawStatBar(statX, statY + 26, 140, 15, TXT_STAT_HAPPINESS, pet.happiness, PINK);
            DrawStatBar(statX, statY + 52, 140, 15, TXT_STAT_ENERGY, pet.energy, GOLD);
            DrawStatBar(statX, statY + 78, 140, 15, TXT_STAT_CLEANLINESS, pet.cleanliness, SKYBLUE);
            DrawStatBar(statX, statY + 104, 140, 15, TXT_STAT_HEALTH, pet.health, RED);

            int btnX = 450;
            if (ButtonEx((Rectangle){ btnX, 390, 300, 38 }, TXT_NAV_KITCHEN, ORANGE, GOLD, 16)) {
                currentScreen = SCREEN_KITCHEN;
            }
            if (ButtonEx((Rectangle){ btnX, 435, 300, 38 }, TXT_NAV_BACKYARD, LIME, GREEN, 16)) {
                currentScreen = SCREEN_BACKYARD;
            }
            if (ButtonEx((Rectangle){ btnX, 480, 300, 38 }, TXT_NAV_BEDROOM, SKYBLUE, BLUE, 16)) {
                currentScreen = SCREEN_BEDROOM;
            }
            if (ButtonEx((Rectangle){ btnX, 525, 300, 38 }, TXT_ACTION_BATH, BEIGE, LIGHTGRAY, 16)) {
                pet.cleanliness = ClampStat(pet.cleanliness + 25, 0, 100);
                snprintf(messageLog, sizeof(messageLog), "%s %s", pet.name, TXT_MSG_CLEANED);
                messageTime = now;
            }

            if (now - messageTime < 3.0) {
                DrawRectangle(40, 330, 420, 30, (Color){ 0, 0, 0, 180 });
                DrawText(messageLog, 50, 337, 16, WHITE);
            }
        }

        // ====================================================================
        // SCREEN 5: KITCHEN SCREEN (Feed)
        // ====================================================================
        else if (currentScreen == SCREEN_KITCHEN) {
            Rectangle bgRec = { 20, 20, 760, 420 };
            DrawAssetOrBox(assets.bgKitchen, bgRec, ORANGE);

            DrawText(TXT_KITCHEN_TITLE, 40, 40, 28, DARKBLUE);
            DrawText(TXT_KITCHEN_SUBTITLE, 40, 75, 18, DARKGRAY);

            int pIdx = PetIndex(pet.type);
            Texture2D avTex = (pIdx >= 0) ? assets.petAvatars[pIdx] : (Texture2D){ 0 };
            DrawPetAvatarAnimated((Rectangle){ 80, 160, 140, 140 }, pet.type, avTex, animFrame);
            DrawStatBar(40, 330, 180, 16, TXT_STAT_HUNGER, pet.hunger, ORANGE);

            if (ButtonEx((Rectangle){ 380, 120, 340, 50 }, TXT_FOOD_SNACK, LIGHTGRAY, SKYBLUE, 16)) {
                pet.hunger = ClampStat(pet.hunger + 10, 0, 100);
                snprintf(messageLog, sizeof(messageLog), "%s %s", pet.name, TXT_MSG_SNACK);
                messageTime = now;
            }
            if (ButtonEx((Rectangle){ 380, 190, 340, 50 }, TXT_FOOD_MEAL, LIGHTGRAY, SKYBLUE, 16)) {
                pet.hunger = ClampStat(pet.hunger + 30, 0, 100);
                pet.energy = ClampStat(pet.energy + 5, 0, 100);
                snprintf(messageLog, sizeof(messageLog), "%s %s", pet.name, TXT_MSG_MEAL);
                messageTime = now;
            }
            if (ButtonEx((Rectangle){ 380, 260, 340, 50 }, TXT_FOOD_DRINK, LIGHTGRAY, SKYBLUE, 16)) {
                pet.energy = ClampStat(pet.energy + 15, 0, 100);
                snprintf(messageLog, sizeof(messageLog), "%s %s", pet.name, TXT_MSG_DRINK);
                messageTime = now;
            }

            if (Button((Rectangle){ 40, 480, 200, 50 }, TXT_BTN_BACK)) {
                currentScreen = SCREEN_MAIN_ROOM;
            }

            if (now - messageTime < 3.0) {
                DrawRectangle(380, 330, 340, 30, (Color){ 0, 0, 0, 180 });
                DrawText(messageLog, 390, 337, 16, WHITE);
            }
        }

        // ====================================================================
        // SCREEN 6: BACKYARD SCREEN (Play)
        // ====================================================================
        else if (currentScreen == SCREEN_BACKYARD) {
            Rectangle bgRec = { 20, 20, 760, 420 };
            DrawAssetOrBox(assets.bgBackyard, bgRec, GREEN);

            DrawText(TXT_BACKYARD_TITLE, 40, 40, 28, DARKBLUE);
            DrawText(TXT_BACKYARD_SUBTITLE, 40, 75, 18, DARKGRAY);

            int pIdx = PetIndex(pet.type);
            Texture2D avTex = (pIdx >= 0) ? assets.petAvatars[pIdx] : (Texture2D){ 0 };
            DrawPetAvatarAnimated((Rectangle){ 80, 160, 140, 140 }, pet.type, avTex, animFrame);
            DrawStatBar(40, 320, 180, 16, TXT_STAT_HAPPINESS, pet.happiness, PINK);
            DrawStatBar(40, 350, 180, 16, TXT_STAT_ENERGY, pet.energy, GOLD);

            if (ButtonEx((Rectangle){ 380, 120, 340, 50 }, TXT_PLAY_BALL, LIGHTGRAY, LIME, 16)) {
                pet.happiness = ClampStat(pet.happiness + 20, 0, 100);
                pet.energy = ClampStat(pet.energy - 10, 0, 100);
                pet.cleanliness = ClampStat(pet.cleanliness - 8, 0, 100);
                snprintf(messageLog, sizeof(messageLog), "%s %s", pet.name, TXT_MSG_BALL);
                messageTime = now;
            }
            if (ButtonEx((Rectangle){ 380, 190, 340, 50 }, TXT_PLAY_FRISBEE, LIGHTGRAY, LIME, 16)) {
                pet.happiness = ClampStat(pet.happiness + 35, 0, 100);
                pet.energy = ClampStat(pet.energy - 20, 0, 100);
                pet.cleanliness = ClampStat(pet.cleanliness - 12, 0, 100);
                snprintf(messageLog, sizeof(messageLog), "%s %s", pet.name, TXT_MSG_FRISBEE);
                messageTime = now;
            }
            if (ButtonEx((Rectangle){ 380, 260, 340, 50 }, TXT_PLAY_WALK, LIGHTGRAY, LIME, 16)) {
                pet.happiness = ClampStat(pet.happiness + 15, 0, 100);
                pet.energy = ClampStat(pet.energy - 5, 0, 100);
                snprintf(messageLog, sizeof(messageLog), "%s %s", pet.name, TXT_MSG_WALK);
                messageTime = now;
            }

            if (Button((Rectangle){ 40, 480, 200, 50 }, TXT_BTN_BACK)) {
                currentScreen = SCREEN_MAIN_ROOM;
            }

            if (now - messageTime < 3.0) {
                DrawRectangle(380, 330, 340, 30, (Color){ 0, 0, 0, 180 });
                DrawText(messageLog, 390, 337, 16, WHITE);
            }
        }

        // ====================================================================
        // SCREEN 7: BEDROOM SCREEN (Rest)
        // ====================================================================
        else if (currentScreen == SCREEN_BEDROOM) {
            Rectangle bgRec = { 20, 20, 760, 420 };
            DrawAssetOrBox(assets.bgBedroom, bgRec, DARKBLUE);

            DrawText(TXT_BEDROOM_TITLE, 40, 40, 28, WHITE);
            DrawText(TXT_BEDROOM_SUBTITLE, 40, 75, 18, LIGHTGRAY);

            int pIdx = PetIndex(pet.type);
            Texture2D avTex = (pIdx >= 0) ? assets.petAvatars[pIdx] : (Texture2D){ 0 };
            DrawPetAvatarAnimated((Rectangle){ 80, 160, 140, 140 }, pet.type, avTex, animFrame);
            DrawText(TXT_SLEEPING_ZZZ, 235, 150, 26, YELLOW);

            DrawStatBar(40, 320, 180, 16, TXT_STAT_ENERGY, pet.energy, GOLD);
            DrawStatBar(40, 350, 180, 16, TXT_STAT_HEALTH, pet.health, RED);

            if (ButtonEx((Rectangle){ 380, 130, 340, 50 }, TXT_REST_NAP, LIGHTGRAY, SKYBLUE, 16)) {
                pet.energy = ClampStat(pet.energy + 25, 0, 100);
                pet.health = ClampStat(pet.health + 5, 0, 100);
                snprintf(messageLog, sizeof(messageLog), "%s %s", pet.name, TXT_MSG_NAP);
                messageTime = now;
            }
            if (ButtonEx((Rectangle){ 380, 210, 340, 50 }, TXT_REST_SLEEP, LIGHTGRAY, SKYBLUE, 16)) {
                pet.energy = ClampStat(pet.energy + 60, 0, 100);
                pet.health = ClampStat(pet.health + 15, 0, 100);
                pet.hunger = ClampStat(pet.hunger - 10, 0, 100);
                snprintf(messageLog, sizeof(messageLog), "%s %s", pet.name, TXT_MSG_SLEEP);
                messageTime = now;
            }

            if (Button((Rectangle){ 40, 480, 200, 50 }, TXT_BTN_BACK)) {
                currentScreen = SCREEN_MAIN_ROOM;
            }

            if (now - messageTime < 3.0) {
                DrawRectangle(380, 290, 340, 30, (Color){ 0, 0, 0, 180 });
                DrawText(messageLog, 390, 297, 16, WHITE);
            }
        }

        EndDrawing();
    }

    // ------------------------------------------------------------------------
    // RESOURCE CLEANUP
    // ------------------------------------------------------------------------
    if (assets.hasCustomFont)           UnloadFont(assets.font);
    if (assets.logo.id > 0)             UnloadTexture(assets.logo);
    for (int i = 0; i < 4; i++) {
        if (assets.petAvatars[i].id > 0) UnloadTexture(assets.petAvatars[i]);
    }
    if (assets.petIllustration.id > 0) UnloadTexture(assets.petIllustration);
    if (assets.bgLivingRoom.id > 0)    UnloadTexture(assets.bgLivingRoom);
    if (assets.bgKitchen.id > 0)       UnloadTexture(assets.bgKitchen);
    if (assets.bgBackyard.id > 0)      UnloadTexture(assets.bgBackyard);
    if (assets.bgBedroom.id > 0)       UnloadTexture(assets.bgBedroom);

    CloseWindow();
    return 0;
}
