#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <stdlib.h>
#include <math.h>

typedef struct {
    Vector2 position;
    float speed;
    float size;
    float swaySpeed;
    float swayAmplitude;
    float angle;
} Snowflake;

Snowflake CreateSnowflake(int screenWidth, int screenHeight) {
    Snowflake flake;
    flake.position = (Vector2){ (float)(rand() % screenWidth), (float)(rand() % screenHeight) };
    flake.speed = (float)((rand() % 200) / 100.0 + 1.0);
    flake.size = (float)((rand() % 300) / 100.0 + 1.5);
    flake.swaySpeed = (float)((rand() % 50) / 1000.0 + 0.02);
    flake.swayAmplitude = (float)((rand() % 200) / 100.0 + 0.5);
    flake.angle = (float)((rand() % 628) / 100.0);
    return flake;
}

int main(void) {
    // display auto config
    int monitor = GetCurrentMonitor();
    int screenWidth = GetMonitorWidth(monitor);
    int screenHeight = GetMonitorHeight(monitor);
    if (screenWidth <= 0) screenWidth = 1280;
    if (screenHeight <= 0) screenHeight = 1024;

    SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TOPMOST);

    InitWindow(screenWidth, screenHeight, "Snowland - a Wayland snowfall simulator");
    SetTargetFPS(60);

	Image icon = LoadImage("icon.png"); 
	SetWindowIcon(icon); 
	UnloadImage(icon); 
	
    float snowDensity = 500.0f;
    float windSpeed = 1.0f;
    float fallSpeed = 1.0f;
    bool showSettings = true;

    int maxSnowflakes = 2000;
    Snowflake *snowflakes = (Snowflake *)malloc(maxSnowflakes * sizeof(Snowflake));
    int currentCount = (int)snowDensity;

    for (int i = 0; i < currentCount; i++) {
        snowflakes[i] = CreateSnowflake(screenWidth, screenHeight);
    }

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_F10)) {
            showSettings = !showSettings;
            if (showSettings) {
                ClearWindowState(FLAG_WINDOW_MOUSE_PASSTHROUGH);
            } else {
                SetWindowState(FLAG_WINDOW_MOUSE_PASSTHROUGH);
            }
        }

        int targetCount = (int)snowDensity;
        if (targetCount > maxSnowflakes) targetCount = maxSnowflakes;
        while (currentCount < targetCount) {
            snowflakes[currentCount++] = CreateSnowflake(screenWidth, screenHeight);
        }
        currentCount = targetCount;

        // snow physics
        for (int i = 0; i < currentCount; i++) {
            snowflakes[i].angle += snowflakes[i].swaySpeed;
            snowflakes[i].position.y += snowflakes[i].speed * fallSpeed;
            snowflakes[i].position.x += (float)sin(snowflakes[i].angle) * snowflakes[i].swayAmplitude + windSpeed;

            if (snowflakes[i].position.y > screenHeight) {
                snowflakes[i].position.y = -10;
                snowflakes[i].position.x = (float)(rand() % screenWidth);
            }
            if (snowflakes[i].position.x < 0) snowflakes[i].position.x = (float)screenWidth;
            if (snowflakes[i].position.x > screenWidth) snowflakes[i].position.x = 0;
        }

        BeginDrawing();
        ClearBackground(BLANK);

        for (int i = 0; i < currentCount; i++) {
            DrawCircleV(snowflakes[i].position, snowflakes[i].size, WHITE);
        }

        if (showSettings) {
            Rectangle panel = { 40, 40, 335, 240 };
            DrawRectangleRec(panel, (Color){ 20, 20, 20, 230 });
            DrawRectangleLinesEx(panel, 1, SKYBLUE);

            DrawText("Snowland settings", (int)panel.x + 15, (int)panel.y + 12, 14, SKYBLUE);
            DrawText("Press F10 to hide/show", (int)panel.x + 15, (int)panel.y + 30, 10, GRAY);

            // density
            DrawText("Density", (int)panel.x + 15, (int)panel.y + 55, 10, LIGHTGRAY);
            GuiSlider((Rectangle){ panel.x + 15, panel.y + 70, 270, 18 }, "", TextFormat("%.0f", snowDensity), &snowDensity, 50.0f, 2000.0f);

            // wind
            DrawText("Wind", (int)panel.x + 15, (int)panel.y + 98, 10, LIGHTGRAY);
            GuiSlider((Rectangle){ panel.x + 15, panel.y + 113, 270, 18 }, "", TextFormat("%.1f", windSpeed), &windSpeed, -5.0f, 5.0f);

            // fall speed
            DrawText("Fall Speed", (int)panel.x + 15, (int)panel.y + 141, 10, LIGHTGRAY);
            GuiSlider((Rectangle){ panel.x + 15, panel.y + 156, 270, 18 }, "", TextFormat("%.1f", fallSpeed), &fallSpeed, 0.2f, 3.0f);

            // reset
            if (GuiButton((Rectangle){ panel.x + 15, panel.y + 190, 100, 28 }, "Reset")) {
                snowDensity = 500.0f;
                windSpeed = 1.0f;
                fallSpeed = 1.0f;
            }
        }

        EndDrawing();
    }

    free(snowflakes);
    CloseWindow();
    return 0;
}
