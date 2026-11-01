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
        else if (screen == SCREEN_SELECT) {
            DrawText("Pilih Hewan Peliharaanmu", 220, 60, 32, DARKGRAY);
            PetType types[4] = { PET_CAT, PET_DOG, PET_RABBIT, PET_NANDA };
            for (int i = 0; i < 4; i++) {
                Rectangle box = { 62.0f + i * 175, 180, 150, 200 };
                DrawRectangleRec(box, PetColor(types[i]));
                if (pet.type == types[i]) DrawRectangleLinesEx(box, 6, DARKBLUE);
                int w = MeasureText(PetLabel(types[i]), 24);
                DrawText(PetLabel(types[i]), (int)(box.x + (box.width - w) / 2), (int)box.y + 215, 24, BLACK);
                if (CheckCollisionPointRec(GetMousePosition(), box) &&
                    IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) pet.type = types[i];
            }
            if (pet.type != PET_NONE &&
                Button((Rectangle){ 325, 510, 150, 50 }, "SELECT")) screen = SCREEN_NAME;
        }
        else if (screen == SCREEN_NAME) {
            DrawText("Beri Nama Petmu", 280, 60, 32, DARKGRAY);
            DrawRectangle(325, 120, 150, 180, PetColor(pet.type));

            int key = GetCharPressed();
            while (key > 0) {
                int len = (int)strlen(pet.name);
                if (key >= 32 && key <= 125 && len < MAX_NAME) {
                    pet.name[len] = (char)key;
                    pet.name[len + 1] = '\0';
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE)) {
                int len = (int)strlen(pet.name);
                if (len > 0) pet.name[len - 1] = '\0';
            }

            DrawRectangle(250, 340, 300, 50, WHITE);
            DrawRectangleLines(250, 340, 300, 50, DARKGRAY);
            DrawText(pet.name, 260, 352, 26, BLACK);
            if (strlen(pet.name) == 0) DrawText("Ketik nama...", 260, 355, 20, LIGHTGRAY);

            if (strlen(pet.name) > 0 &&
                Button((Rectangle){ 325, 430, 150, 50 }, "CONFIRM")) screen = SCREEN_GAME;
        }
        else if (screen == SCREEN_GAME) {
            ClearBackground((Color){ 255, 240, 220, 255 });
            DrawText(pet.name, 30, 20, 36, DARKBLUE);
            DrawText(PetLabel(pet.type), 30, 62, 20, DARKGRAY);
            DrawRectangle(325, 200, 150, 180, PetColor(pet.type));

            DrawText(TextFormat("Lapar: %d", pet.hunger), 30, 120, 20, BLACK);
            DrawText(TextFormat("Senang: %d", pet.happiness), 30, 150, 20, BLACK);
            DrawText(TextFormat("Energi: %d", pet.energy), 30, 180, 20, BLACK);

            Button((Rectangle){ 200, 500, 120, 50 }, "FEED");
            Button((Rectangle){ 340, 500, 120, 50 }, "PLAY");
            Button((Rectangle){ 480, 500, 120, 50 }, "REST");
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
