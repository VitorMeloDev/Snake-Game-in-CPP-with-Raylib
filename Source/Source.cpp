#include <raylib.h>

#define SCREEN_X 750   
#define SCREEN_Y 750
#define WINDOW_TITLE "Snake Game - VM Games"

Color green = { 173, 204, 96, 255 };
Color darkGreen = { 43, 51, 24, 255 };

int main()
{
    InitWindow(SCREEN_X, SCREEN_Y, WINDOW_TITLE);

    // Main game loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();    
    }
    CloseWindow();  

    return 0;
}