#include <raylib.h>
#include <iostream>

#define WINDOW_TITLE "Snake Game - VM Games"

Color green = { 173, 204, 96, 255 };
Color darkGreen = { 43, 51, 24, 255 };

int cellSize = 30;
int cellCount = 25;

class Food
{
public:
    Vector2 position;
	Texture2D texture;
    Food()
    {
        Image image = LoadImage("Graphics/food.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomPos();
	}

    ~Food()
    {
		UnloadTexture(texture);
    }

    void Draw()
    {
        DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
	}

    Vector2 GenerateRandomPos()
    {
		float x = GetRandomValue(0, cellCount - 1);
		float y = GetRandomValue(0, cellCount - 1);

		return { x, y };
    }
};

int main()
{
    InitWindow(cellSize*cellCount, cellSize * cellCount, WINDOW_TITLE);
	SetTargetFPS(60);

	Food food = Food();

    // Main game loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
		food.Draw();
        ClearBackground(green);
        EndDrawing();    
    }
    CloseWindow();  

    return 0;
}