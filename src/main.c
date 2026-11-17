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
#include <math.h>
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
    SCREEN_BACKYARD,   // Backyard (play)
    SCREEN_BATHROOM    // Bathroom (clean/shower)
} Screen;

// ----------------------------------------------------------------------------
// ASSET MANAGEMENT STRUCT
// Holds loaded textures and custom font.
// ----------------------------------------------------------------------------
typedef struct {
    Font font;
    bool hasCustomFont;
    Texture2D logo;
    Texture2D petAvatars[3];
    Texture2D petHappyAvatars[3];
    Texture2D petSadAvatars[3];
    Texture2D petSleepAvatars[3];
    Texture2D petIllustration;
    Texture2D bgLivingRoom;
    Texture2D bgKitchen;
    Texture2D bgBackyard;
    Texture2D bgBedroom;
    Texture2D bgBathroom;
} GameAssets;

// ----------------------------------------------------------------------------
// HELPER FUNCTIONS
// ----------------------------------------------------------------------------

static Font g_gameFont = { 0 };
static bool g_hasCustomFont = false;

static void AppDrawText(const char *text, int posX, int posY, int fontSize, Color color) {
    if (g_hasCustomFont && g_gameFont.texture.id > 0) {
        DrawTextEx(g_gameFont, text, (Vector2){ (float)posX, (float)posY }, (float)fontSize, 1.0f, color);
    } else {
        (DrawText)(text, posX, posY, fontSize, color);
    }
}

static int AppMeasureText(const char *text, int fontSize) {
    if (g_hasCustomFont && g_gameFont.texture.id > 0) {
        return (int)MeasureTextEx(g_gameFont, text, (float)fontSize, 1.0f).x;
    }
    return (MeasureText)(text, fontSize);
}

#define DrawText AppDrawText
#define MeasureText AppMeasureText

static int ClampStat(int value, int minVal, int maxVal) {
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

static int PetOverallStat(const Pet *pet) {
    return (pet->hunger + pet->happiness + pet->energy + pet->cleanliness + pet->health) / 5;
}

static const char *PetMood(int overallStat) {
    if (overallStat >= 67) return TXT_MOOD_HAPPY;
    if (overallStat >= 34) return TXT_MOOD_NEUTRAL;
    return TXT_MOOD_SAD;
}

static Color PetMoodColor(int overallStat) {
    if (overallStat >= 67) return GREEN;
    if (overallStat >= 34) return YELLOW;
    return RED;
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
        default:         return "";
    }
}

static Color PetColor(PetType t) {
    switch (t) {
        case PET_CAT:    return ORANGE;
        case PET_DOG:    return BROWN;
        case PET_RABBIT: return PINK;
        default:         return GRAY;
    }
}

static int PetIndex(PetType t) {
    switch (t) {
        case PET_CAT:    return 0;
        case PET_DOG:    return 1;
        case PET_RABBIT: return 2;
        default:         return -1;
    }
}

static Texture2D PetMoodTexture(const Pet *pet, const GameAssets *assets, int petIndex) {
    if (petIndex < 0) return (Texture2D){ 0 };
    if (PetOverallStat(pet) < 34 && assets->petSadAvatars[petIndex].id > 0) {
        return assets->petSadAvatars[petIndex];
    }
    return assets->petAvatars[petIndex];
}

static Texture2D PetSleepTexture(const GameAssets *assets, int petIndex) {
    if (petIndex >= 0 && assets->petSleepAvatars[petIndex].id > 0) {
        return assets->petSleepAvatars[petIndex];
    }
    return petIndex >= 0 ? assets->petAvatars[petIndex] : (Texture2D){ 0 };
}

static void DrawPetConditionEffects(Rectangle r, const Pet *pet, double now) {
    if (pet->cleanliness >= PET_DIRTY_THRESHOLD) return;

    DrawCircleV((Vector2){ r.x + r.width * 0.30f, r.y + r.height * 0.34f }, 4, (Color){ 120, 75, 35, 185 });
    DrawCircleV((Vector2){ r.x + r.width * 0.68f, r.y + r.height * 0.57f }, 3, (Color){ 120, 75, 35, 185 });
    DrawCircleV((Vector2){ r.x + r.width * 0.45f, r.y + r.height * 0.73f }, 2, (Color){ 120, 75, 35, 185 });

    for (int i = 0; i < 3; i++) {
        float angle = (float)now * 3.0f + (float)i * 2.1f;
        float cx = r.x + r.width * 0.5f + cosf(angle) * (r.width * 0.62f);
        float cy = r.y + r.height * 0.5f + sinf(angle * 1.3f) * (r.height * 0.48f);
        DrawEllipse((int)cx, (int)cy, 4, 2, DARKGRAY);
        DrawCircle((int)cx - 3, (int)cy - 2, 2, (Color){ 225, 235, 245, 220 });
        DrawCircle((int)cx + 3, (int)cy - 2, 2, (Color){ 225, 235, 245, 220 });
    }
}

static void DrawBathFoam(Rectangle petRec, double elapsed) {
    float bob = sinf((float)elapsed * 5.0f) * 2.0f;
    Color foam = (Color){ 245, 252, 255, 220 };
    DrawCircleV((Vector2){ petRec.x + 12, petRec.y + 42 + bob }, 17, foam);
    DrawCircleV((Vector2){ petRec.x + 27, petRec.y + 25 + bob }, 19, foam);
    DrawCircleV((Vector2){ petRec.x + petRec.width - 16, petRec.y + 35 - bob }, 18, foam);
    DrawCircleV((Vector2){ petRec.x + petRec.width - 4, petRec.y + 59 - bob }, 14, foam);
    DrawCircleV((Vector2){ petRec.x + 24, petRec.y + petRec.height - 18 - bob }, 15, foam);
    DrawCircleV((Vector2){ petRec.x + petRec.width - 22, petRec.y + petRec.height - 16 + bob }, 17, foam);
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

static void DrawRoomTitle(const char *title, int x, int y) {
    int textW = MeasureText(title, 20);
    Rectangle pill = { (float)x - 10, (float)y - 6, (float)textW + 20, 32 };
    DrawRectangleRounded(pill, 0.4f, 4, (Color){ 0, 0, 0, 160 });
    DrawRectangleRoundedLines(pill, 0.4f, 4, (Color){ 255, 255, 255, 80 });
    DrawText(title, x, y, 20, WHITE);
}

static void DrawMessagePopup(const char *msg, double now, double msgTime) {
    if (now - msgTime < 3.0 && msg != NULL && msg[0] != '\0') {
        int textW = MeasureText(msg, 16);
        int popW = textW + 28;
        if (popW < 240) popW = 240;
        if (popW > 700) popW = 700;
        Rectangle popRec = { 40, 325, (float)popW, 32 };
        DrawRectangleRounded(popRec, 0.35f, 4, (Color){ 0, 0, 0, 195 });
        DrawRectangleRoundedLines(popRec, 0.35f, 4, (Color){ 255, 255, 255, 90 });
        DrawText(msg, 54, 332, 16, WHITE);
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
 * Increased label offset to prevent text overlap with custom fonts.
 */
static void DrawStatBar(int x, int y, int width, int height, const char *label, int value, Color barColor) {
    DrawText(label, x, y, 16, BLACK);
    int labelOffset = 135;
    Rectangle bg = { (float)x + labelOffset, (float)y, (float)width, (float)height };
    DrawRectangleRec(bg, LIGHTGRAY);
    DrawRectangleLinesEx(bg, 1, DARKGRAY);

    float fillWidth = (float)width * ((float)value / 100.0f);
    Rectangle fill = { (float)x + labelOffset, (float)y, fillWidth, (float)height };
    DrawRectangleRec(fill, barColor);

    DrawText(TextFormat("%d/100", value), x + labelOffset + width + 8, y, 14, DARKGRAY);
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
        assets.font = LoadFontEx(PATH_FONT, 72, NULL, 0);
        SetTextureFilter(assets.font.texture, TEXTURE_FILTER_BILINEAR);
        assets.hasCustomFont = true;
        g_gameFont = assets.font;
        g_hasCustomFont = true;
    }

    assets.logo            = LoadTextureWithFallback(PATH_LOGO, FileExists("petopia.png") ? "petopia.png" : "assets/logo.png");
    assets.petAvatars[0]   = LoadTextureWithFallback(PATH_PET_CAT, FileExists("cat selection.png") ? "cat selection.png" : "assets/cat.png");
    assets.petAvatars[1]   = LoadTextureWithFallback(PATH_PET_DOG, FileExists("dog selection.png") ? "dog selection.png" : "assets/dog.png");
    assets.petAvatars[2]   = LoadTextureWithFallback(PATH_PET_RABBIT, FileExists("rabbit selection.png") ? "rabbit selection.png" : "assets/rabbit.png");
    assets.petHappyAvatars[0] = LoadTextureWithFallback(PATH_PET_CAT_HAPPY, NULL);
    assets.petHappyAvatars[1] = LoadTextureWithFallback(PATH_PET_DOG_HAPPY, NULL);
    assets.petHappyAvatars[2] = LoadTextureWithFallback(PATH_PET_RABBIT_HAPPY, NULL);
    assets.petSadAvatars[0] = LoadTextureWithFallback(PATH_PET_CAT_SAD, NULL);
    assets.petSadAvatars[1] = LoadTextureWithFallback(PATH_PET_DOG_SAD, NULL);
    assets.petSadAvatars[2] = LoadTextureWithFallback(PATH_PET_RABBIT_SAD, NULL);
    assets.petSleepAvatars[0] = LoadTextureWithFallback(PATH_PET_CAT_SLEEP, NULL);
    assets.petSleepAvatars[1] = LoadTextureWithFallback(PATH_PET_DOG_SLEEP, NULL);
    assets.petSleepAvatars[2] = LoadTextureWithFallback(PATH_PET_RABBIT_SLEEP, NULL);
    assets.petIllustration = LoadTextureWithFallback(PATH_PET_ILLUST, "assets/pet_illustration.png");
    assets.bgLivingRoom    = LoadTextureWithFallback(PATH_BG_LIVINGROOM, "assets/bg_livingroom.png");
    assets.bgKitchen       = LoadTextureWithFallback(PATH_BG_KITCHEN, "assets/bg_kitchen.png");
    assets.bgBackyard      = LoadTextureWithFallback(PATH_BG_BACKYARD, FileExists("assets/images/backgrounds/backyard") ? "assets/images/backgrounds/backyard" : "assets/bg_backyard.png");
    assets.bgBedroom       = LoadTextureWithFallback(PATH_BG_BEDROOM, FileExists("assets/images/backgrounds/bedroom") ? "assets/images/backgrounds/bedroom" : "assets/bg_bedroom.png");
    assets.bgBathroom      = LoadTextureWithFallback(PATH_BG_BATHROOM, "assets/bg_bathroom.png");

    Screen currentScreen = SCREEN_WELCOME;
    Pet pet = { PET_NONE, "", 80, 80, 80, 80, 90 };

    double lastDecayTime = GetTime();
    char messageLog[64] = TXT_MSG_WELCOME;
    double messageTime = GetTime();

    // Animation frame tracking
    int animFrame = 0;
    double lastAnimTime = GetTime();
    double happyExpressionStart = GetTime();
    double pettingStart = -1.0;
    double sadShakeStart = -1.0;
    bool sadMoodActive = false;
    bool bathActive = false;
    bool fullBathActive = false;
    double bathStart = 0.0;
    double bathDuration = 0.0;

    // ------------------------------------------------------------------------
    // MAIN LOOP
    // ------------------------------------------------------------------------
    while (!WindowShouldClose()) {
        double now = GetTime();

        if (bathActive && now - bathStart >= bathDuration) {
            pet.cleanliness = ClampStat(pet.cleanliness + (fullBathActive ? 50 : 25), 0, 100);
            if (fullBathActive) {
                pet.happiness = ClampStat(pet.happiness + 15, 0, 100);
                pet.health = ClampStat(pet.health + 10, 0, 100);
                pet.energy = ClampStat(pet.energy - 10, 0, 100);
                snprintf(messageLog, sizeof(messageLog), "%s %s", pet.name, TXT_MSG_CLEANED);
            } else {
                pet.happiness = ClampStat(pet.happiness + 5, 0, 100);
                snprintf(messageLog, sizeof(messageLog), "%s %s", pet.name, TXT_MSG_SHOWERED);
            }
            messageTime = now;
            bathActive = false;
        }

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

        bool isSad = PetOverallStat(&pet) < 34;
        if (isSad && !sadMoodActive) {
            sadShakeStart = now;
        } else if (!isSad) {
            sadShakeStart = -1.0;
        }
        sadMoodActive = isSad;

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

            Rectangle logoBox = { 120, 160, 560, 180 };
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

            PetType types[3] = { PET_CAT, PET_DOG, PET_RABBIT };

            for (int i = 0; i < 3; i++) {
                Rectangle card = { 140.0f + i * 180, 120, 160, 330 };
                
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

            if (currentScreen == SCREEN_SELECT && pet.type != PET_NONE) {
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

            Rectangle illustRec = { 290, 70, 220, 210 };
            int pIdx = PetIndex(pet.type);
            Texture2D pTex = (pIdx >= 0) ? assets.petAvatars[pIdx] : (Texture2D){ 0 };
            if (assets.petIllustration.id > 0) pTex = assets.petIllustration;

            DrawAssetOrBox(pTex, illustRec, PetColor(pet.type));
            if (pTex.id == 0) {
                DrawPetAvatarAnimated((Rectangle){ 330, 105, 140, 140 }, pet.type, (Texture2D){ 0 }, animFrame);
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

            // Clean title HUD in top-left
            DrawRoomTitle(TextFormat("%s si %s", pet.name, PetLabel(pet.type)), 40, 35);

            // Pet placed naturally on the floor near drawers / aquarium (not floating)
            int pIdx = PetIndex(pet.type);
            Rectangle petRec = { 360, 215, 115, 115 };
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                CheckCollisionPointRec(GetMousePosition(), petRec)) {
                pettingStart = now;
            }
            if (pettingStart >= 0.0 && now - pettingStart < PETTING_DURATION) {
                petRec.x += sinf((float)(now - pettingStart) * 42.0f) * 3.0f;
            }
            if (sadShakeStart >= 0.0 && now - sadShakeStart < PET_SAD_SHAKE_DURATION) {
                petRec.x += sinf((float)(now - sadShakeStart) * 18.0f) * 2.0f;
            }
            Texture2D avTex = PetMoodTexture(&pet, &assets, pIdx);
            int overallStat = PetOverallStat(&pet);
            double happyCycleTime = fmod(now - happyExpressionStart, PET_HAPPY_INTERVAL);
            if (overallStat >= 67 && happyCycleTime < PET_HAPPY_DURATION &&
                pIdx >= 0 && assets.petHappyAvatars[pIdx].id > 0) {
                avTex = assets.petHappyAvatars[pIdx];
            }
            DrawPetAvatarAnimated(petRec, pet.type, avTex, animFrame);
            DrawPetConditionEffects(petRec, &pet, now);
            
            int nameW = MeasureText(pet.name, 22);
            DrawText(pet.name, 360 + (115 - nameW) / 2, 185, 22, DARKBLUE);

            int statX = 40;
            int statY = 390;
            DrawStatBar(statX, statY, 140, 15, TXT_STAT_HUNGER, pet.hunger, ORANGE);
            DrawStatBar(statX, statY + 26, 140, 15, TXT_STAT_HAPPINESS, pet.happiness, PINK);
            DrawStatBar(statX, statY + 52, 140, 15, TXT_STAT_ENERGY, pet.energy, GOLD);
            DrawStatBar(statX, statY + 78, 140, 15, TXT_STAT_CLEANLINESS, pet.cleanliness, SKYBLUE);
            DrawStatBar(statX, statY + 104, 140, 15, TXT_STAT_HEALTH, pet.health, RED);
            DrawText(TextFormat("%s: %d/100", TXT_OVERALL_STATS, overallStat),
                     statX, statY + 136, 16, DARKBLUE);
            DrawText(PetMood(overallStat), statX, statY + 156, 16, PetMoodColor(overallStat));

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
                currentScreen = SCREEN_BATHROOM;
            }

            DrawMessagePopup(messageLog, now, messageTime);
        }

        // ====================================================================
        // SCREEN 5: KITCHEN SCREEN (Feed)
        // ====================================================================
        else if (currentScreen == SCREEN_KITCHEN) {
            Rectangle bgRec = { 20, 20, 760, 350 };
            DrawAssetOrBox(assets.bgKitchen, bgRec, ORANGE);

            DrawRoomTitle(TXT_KITCHEN_TITLE, 40, 35);

            // Pet beside food & water bowls
            int pIdx = PetIndex(pet.type);
            Rectangle petRec = { 190, 190, 130, 130 };
            Texture2D avTex = PetMoodTexture(&pet, &assets, pIdx);
            DrawPetAvatarAnimated(petRec, pet.type, avTex, animFrame);
            DrawPetConditionEffects(petRec, &pet, now);

            // Left side: Stat bars & Back button (x = 40)
            int statX = 40;
            DrawStatBar(statX, 395, 140, 15, TXT_STAT_HUNGER, pet.hunger, ORANGE);
            DrawStatBar(statX, 430, 140, 15, TXT_STAT_ENERGY, pet.energy, GOLD);

            if (Button((Rectangle){ statX, 480, 200, 46 }, TXT_BTN_BACK)) {
                currentScreen = SCREEN_MAIN_ROOM;
            }

            // Right side: Action buttons proportional at x = 450 (no overlap with stat bars!)
            int btnX = 450;
            if (ButtonEx((Rectangle){ btnX, 390, 300, 42 }, TXT_FOOD_SNACK, LIGHTGRAY, SKYBLUE, 16)) {
                pet.hunger = ClampStat(pet.hunger + 10, 0, 100);
                snprintf(messageLog, sizeof(messageLog), "%s %s", pet.name, TXT_MSG_SNACK);
                messageTime = now;
            }
            if (ButtonEx((Rectangle){ btnX, 438, 300, 42 }, TXT_FOOD_MEAL, LIGHTGRAY, SKYBLUE, 16)) {
                pet.hunger = ClampStat(pet.hunger + 30, 0, 100);
                pet.energy = ClampStat(pet.energy + 5, 0, 100);
                snprintf(messageLog, sizeof(messageLog), "%s %s", pet.name, TXT_MSG_MEAL);
                messageTime = now;
            }
            if (ButtonEx((Rectangle){ btnX, 486, 300, 42 }, TXT_FOOD_DRINK, LIGHTGRAY, SKYBLUE, 16)) {
                pet.energy = ClampStat(pet.energy + 15, 0, 100);
                snprintf(messageLog, sizeof(messageLog), "%s %s", pet.name, TXT_MSG_DRINK);
                messageTime = now;
            }

            DrawMessagePopup(messageLog, now, messageTime);
        }

        // ====================================================================
        // SCREEN 6: BACKYARD SCREEN (Play)
        // ====================================================================
        else if (currentScreen == SCREEN_BACKYARD) {
            Rectangle bgRec = { 20, 20, 760, 350 };
            DrawAssetOrBox(assets.bgBackyard, bgRec, GREEN);

            DrawRoomTitle(TXT_BACKYARD_TITLE, 40, 35);

            int pIdx = PetIndex(pet.type);
            Rectangle petRec = { 315, 180, 130, 130 };
            Texture2D avTex = PetMoodTexture(&pet, &assets, pIdx);
            DrawPetAvatarAnimated(petRec, pet.type, avTex, animFrame);
            DrawPetConditionEffects(petRec, &pet, now);

            // Left side: Stat bars & Back button
            int statX = 40;
            DrawStatBar(statX, 395, 140, 15, TXT_STAT_HAPPINESS, pet.happiness, PINK);
            DrawStatBar(statX, 430, 140, 15, TXT_STAT_ENERGY, pet.energy, GOLD);

            if (Button((Rectangle){ statX, 480, 200, 46 }, TXT_BTN_BACK)) {
                currentScreen = SCREEN_MAIN_ROOM;
            }

            // Right side: Action buttons at x = 450
            int btnX = 450;
            if (ButtonEx((Rectangle){ btnX, 390, 300, 42 }, TXT_PLAY_BALL, LIGHTGRAY, LIME, 16)) {
                pet.happiness = ClampStat(pet.happiness + 20, 0, 100);
                pet.energy = ClampStat(pet.energy - 10, 0, 100);
                pet.cleanliness = ClampStat(pet.cleanliness - 8, 0, 100);
                snprintf(messageLog, sizeof(messageLog), "%s %s", pet.name, TXT_MSG_BALL);
                messageTime = now;
            }
            if (ButtonEx((Rectangle){ btnX, 438, 300, 42 }, TXT_PLAY_FRISBEE, LIGHTGRAY, LIME, 16)) {
                pet.happiness = ClampStat(pet.happiness + 35, 0, 100);
                pet.energy = ClampStat(pet.energy - 20, 0, 100);
                pet.cleanliness = ClampStat(pet.cleanliness - 12, 0, 100);
                snprintf(messageLog, sizeof(messageLog), "%s %s", pet.name, TXT_MSG_FRISBEE);
                messageTime = now;
            }
            if (ButtonEx((Rectangle){ btnX, 486, 300, 42 }, TXT_PLAY_WALK, LIGHTGRAY, LIME, 16)) {
                pet.happiness = ClampStat(pet.happiness + 15, 0, 100);
                pet.energy = ClampStat(pet.energy - 5, 0, 100);
                snprintf(messageLog, sizeof(messageLog), "%s %s", pet.name, TXT_MSG_WALK);
                messageTime = now;
            }

            DrawMessagePopup(messageLog, now, messageTime);
        }

        // ====================================================================
        // SCREEN 7: BEDROOM SCREEN (Rest)
        // ====================================================================
        else if (currentScreen == SCREEN_BEDROOM) {
            Rectangle bgRec = { 20, 20, 760, 350 };
            DrawAssetOrBox(assets.bgBedroom, bgRec, DARKBLUE);

            DrawRoomTitle(TXT_BEDROOM_TITLE, 40, 35);

            int pIdx = PetIndex(pet.type);
            Rectangle petRec = { 340, 155, 120, 120 };
            Texture2D avTex = PetSleepTexture(&assets, pIdx);
            DrawPetAvatarAnimated(petRec, pet.type, avTex, animFrame);
            DrawPetConditionEffects(petRec, &pet, now);
            DrawText(TXT_SLEEPING_ZZZ, 450, 125, 26, YELLOW);

            // Left side: Stat bars & Back button
            int statX = 40;
            DrawStatBar(statX, 395, 140, 15, TXT_STAT_ENERGY, pet.energy, GOLD);
            DrawStatBar(statX, 430, 140, 15, TXT_STAT_HEALTH, pet.health, RED);

            if (Button((Rectangle){ statX, 480, 200, 46 }, TXT_BTN_BACK)) {
                currentScreen = SCREEN_MAIN_ROOM;
            }

            // Right side: Action buttons at x = 450
            int btnX = 450;
            if (ButtonEx((Rectangle){ btnX, 400, 300, 46 }, TXT_REST_NAP, LIGHTGRAY, SKYBLUE, 16)) {
                pet.energy = ClampStat(pet.energy + 25, 0, 100);
                pet.health = ClampStat(pet.health + 5, 0, 100);
                snprintf(messageLog, sizeof(messageLog), "%s %s", pet.name, TXT_MSG_NAP);
                messageTime = now;
            }
            if (ButtonEx((Rectangle){ btnX, 460, 300, 46 }, TXT_REST_SLEEP, LIGHTGRAY, SKYBLUE, 16)) {
                pet.energy = ClampStat(pet.energy + 60, 0, 100);
                pet.health = ClampStat(pet.health + 15, 0, 100);
                pet.hunger = ClampStat(pet.hunger - 10, 0, 100);
                snprintf(messageLog, sizeof(messageLog), "%s %s", pet.name, TXT_MSG_SLEEP);
                messageTime = now;
            }

            DrawMessagePopup(messageLog, now, messageTime);
        }

        // ====================================================================
        // SCREEN 8: BATHROOM SCREEN (Clean / Shower)
        // ====================================================================
        else if (currentScreen == SCREEN_BATHROOM) {
            Rectangle bgRec = { 20, 20, 760, 350 };
            DrawAssetOrBox(assets.bgBathroom, bgRec, SKYBLUE);

            DrawRoomTitle(TXT_BATHROOM_TITLE, 40, 35);

            int pIdx = PetIndex(pet.type);
            Rectangle petRec = { 280, 165, 120, 120 };
            Texture2D avTex = PetMoodTexture(&pet, &assets, pIdx);
            DrawPetAvatarAnimated(petRec, pet.type, avTex, animFrame);
            DrawPetConditionEffects(petRec, &pet, now);
            if (bathActive) {
                DrawBathFoam(petRec, now - bathStart);
            }

            // Left side: Stat bars & Back button
            int statX = 40;
            DrawStatBar(statX, 395, 140, 15, TXT_STAT_CLEANLINESS, pet.cleanliness, SKYBLUE);
            DrawStatBar(statX, 430, 140, 15, TXT_STAT_HEALTH, pet.health, RED);

            if (Button((Rectangle){ statX, 480, 200, 46 }, TXT_BTN_BACK)) {
                currentScreen = SCREEN_MAIN_ROOM;
            }

            // Right side: Action buttons at x = 450
            int btnX = 450;
            if (ButtonEx((Rectangle){ btnX, 390, 300, 42 }, TXT_BATH_SHOWER, LIGHTGRAY, SKYBLUE, 16) && !bathActive) {
                bathActive = true;
                fullBathActive = false;
                bathStart = now;
                bathDuration = BATH_SHOWER_DURATION;
            }
            if (ButtonEx((Rectangle){ btnX, 438, 300, 42 }, TXT_BATH_BRUSH, LIGHTGRAY, SKYBLUE, 16) && !bathActive) {
                pet.cleanliness = ClampStat(pet.cleanliness + 15, 0, 100);
                pet.happiness = ClampStat(pet.happiness + 10, 0, 100);
                snprintf(messageLog, sizeof(messageLog), "%s %s", pet.name, TXT_MSG_BRUSHED);
                messageTime = now;
            }
            if (ButtonEx((Rectangle){ btnX, 486, 300, 42 }, TXT_BATH_FULL, LIGHTGRAY, SKYBLUE, 16) && !bathActive) {
                bathActive = true;
                fullBathActive = true;
                bathStart = now;
                bathDuration = BATH_FULL_DURATION;
            }
            if (bathActive) {
                float progress = (float)((now - bathStart) / bathDuration);
                if (progress > 1.0f) progress = 1.0f;
                DrawText(TXT_BATH_IN_PROGRESS, btnX, 535, 16, DARKBLUE);
                DrawRectangleRec((Rectangle){ (float)btnX, 558, 300, 12 }, LIGHTGRAY);
                DrawRectangleRec((Rectangle){ (float)btnX, 558, 300.0f * progress, 12 }, SKYBLUE);
            }

            DrawMessagePopup(messageLog, now, messageTime);
        }

        EndDrawing();
    }

    // ------------------------------------------------------------------------
    // RESOURCE CLEANUP
    // ------------------------------------------------------------------------
    if (assets.hasCustomFont)           UnloadFont(assets.font);
    if (assets.logo.id > 0)             UnloadTexture(assets.logo);
    for (int i = 0; i < 3; i++) {
        if (assets.petAvatars[i].id > 0) UnloadTexture(assets.petAvatars[i]);
        if (assets.petHappyAvatars[i].id > 0) UnloadTexture(assets.petHappyAvatars[i]);
        if (assets.petSadAvatars[i].id > 0) UnloadTexture(assets.petSadAvatars[i]);
        if (assets.petSleepAvatars[i].id > 0) UnloadTexture(assets.petSleepAvatars[i]);
    }
    if (assets.petIllustration.id > 0) UnloadTexture(assets.petIllustration);
    if (assets.bgLivingRoom.id > 0)    UnloadTexture(assets.bgLivingRoom);
    if (assets.bgKitchen.id > 0)       UnloadTexture(assets.bgKitchen);
    if (assets.bgBackyard.id > 0)      UnloadTexture(assets.bgBackyard);
    if (assets.bgBedroom.id > 0)       UnloadTexture(assets.bgBedroom);
    if (assets.bgBathroom.id > 0)      UnloadTexture(assets.bgBathroom);

    CloseWindow();
    return 0;
}
