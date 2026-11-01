#include <stdbool.h>
#include <string.h>
#include "raylib.h"
#include "pet.h"

typedef enum { SCREEN_WELCOME, SCREEN_SELECT, SCREEN_NAME, SCREEN_GAME } Screen;

static bool Button(Rectangle r, const char *text) {
    bool hover = CheckCollisionPointRec(GetMousePosition(), r);
    DrawRectangleRec(r, hover ? SKYBLUE : LIGHTGRAY);
    DrawRectangleLinesEx(r, 2, DARKGRAY);
    int w = MeasureText(text, 20);
    DrawText(text, (int)(r.x + (r.width - w) / 2), (int)(r.y + (r.height - 20) / 2), 20, BLACK);
    return hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

static const char *PetLabel(PetType t) {
    if (t == PET_CAT) return "Kucing";
    if (t == PET_DOG) return "Anjing";
    if (t == PET_RABBIT) return "Kelinci";
    if (t == PET_NANDA) return "NANDA";
    return "";
}

static Color PetColor(PetType t) {
    if (t == PET_CAT) return ORANGE;
    if (t == PET_DOG) return BROWN;
    if (t == PET_RABBIT) return PINK;
    if (t == PET_NANDA) return BLUE;
    return GRAY;
}

int main(void) {
    InitWindow(800, 600, "PETOPIA");
    SetTargetFPS(60);

    Screen screen = SCREEN_WELCOME;
    Pet pet = { PET_NONE, "", 100, 100, 100 };

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (screen == SCREEN_WELCOME) {
            DrawText("PETOPIA", 290, 180, 60, DARKBLUE);
            DrawText("Rawat hewan peliharaan virtualmu!", 220, 270, 24, DARKGRAY);
            if (Button((Rectangle){ 325, 350, 150, 50 }, "START")) screen = SCREEN_SELECT;
        }
        else if (screen ==
